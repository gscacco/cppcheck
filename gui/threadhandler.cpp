/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */


#include "threadhandler.h"
#include <QDebug>

ThreadHandler::ThreadHandler(QObject *parent) :
        QObject(parent),
        mRunningThreadCount(0)
{
    SetThreadCount(1);
}

ThreadHandler::~ThreadHandler()
{
    RemoveThreads();
}

void ThreadHandler::ClearFiles()
{
    mLastFiles.clear();
    mResults.ClearFiles();
}

void ThreadHandler::SetFiles(const QStringList &files)
{
    mResults.SetFiles(files);
    mLastFiles = files;
}

void ThreadHandler::Check(Settings settings, bool recheck)
{
    if (recheck && mRunningThreadCount == 0)
    {
        mResults.SetFiles(mLastFiles);
    }

    if (mResults.GetFileCount() == 0 || mRunningThreadCount > 0 || settings._jobs <= 0)
    {
        qDebug() << "Can't start checking if there's no files to check or if check is in progress.";
        emit Done();
        return;
    }

    SetThreadCount(settings._jobs);

    mRunningThreadCount = mThreads.size();

    if (mResults.GetFileCount() < mRunningThreadCount)
    {
        mRunningThreadCount = mResults.GetFileCount();
    }

    for (int i = 0; i < mRunningThreadCount; i++)
    {
        mThreads[i]->Check(settings);
    }
}

bool ThreadHandler::IsChecking() const
{
    return mRunningThreadCount > 0;
}

void ThreadHandler::SetThreadCount(const int count)
{
    if (mRunningThreadCount > 0 ||
        count == mThreads.size() ||
        count <= 0)
    {
        return;
    }

    //Remove unused old threads
    RemoveThreads();
    //Create new threads
    for (int i = mThreads.size(); i < count; i++)
    {
        mThreads << new CheckThread(mResults);
        connect(mThreads.last(), SIGNAL(Done()),
                this, SLOT(ThreadDone()));
        connect(mThreads.last(), SIGNAL(FileChecked(const QString &)),
                &mResults, SLOT(FileChecked(const QString &)));
    }

}


void ThreadHandler::RemoveThreads()
{
    for (int i = 0; i < mThreads.size(); i++)
    {
        mThreads[i]->terminate();
        disconnect(mThreads.last(), SIGNAL(Done()),
                   this, SLOT(ThreadDone()));
        disconnect(mThreads.last(), SIGNAL(FileChecked(const QString &)),
                   &mResults, SLOT(FileChecked(const QString &)));

        delete mThreads[i];
    }

    mThreads.clear();
}

void ThreadHandler::ThreadDone()
{
    mRunningThreadCount--;
    if (mRunningThreadCount == 0)
    {
        emit Done();
    }
}

void ThreadHandler::Stop()
{
    for (int i = 0; i < mThreads.size(); i++)
    {
        mThreads[i]->stop();
    }
}

void ThreadHandler::Initialize(ResultsView *view)
{

    connect(&mResults, SIGNAL(Progress(int, int)),
            view, SLOT(Progress(int, int)));

    connect(&mResults, SIGNAL(Error(const QString &,
                                    const QString &,
                                    const QString &,
                                    const QStringList &,
                                    const QVariantList &,
                                    const QString &)),
            view, SLOT(Error(const QString &,
                             const QString &,
                             const QString &,
                             const QStringList &,
                             const QVariantList &,
                             const QString &)));

}

void ThreadHandler::LoadSettings(QSettings &settings)
{
    SetThreadCount(settings.value(SETTINGS_CHECK_THREADS, 1).toInt());
}

void ThreadHandler::SaveSettings(QSettings &settings)
{
    settings.setValue(SETTINGS_CHECK_THREADS, mThreads.size());
}

