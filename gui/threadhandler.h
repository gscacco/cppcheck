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


#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <QObject>
#include <QStringList>
#include "../src/settings.h"
#include "../src/cppcheck.h"
#include "threadresult.h"
#include "checkthread.h"
#include "resultsview.h"

/// @addtogroup GUI
/// @{


/**
* @brief This class handles creating threadresult and starting threads
*
*/
class ThreadHandler : public QObject
{
    Q_OBJECT
public:
    ThreadHandler(QObject *parent = 0);
    virtual ~ThreadHandler();

    /**
    * @brief Set the number of threads to use
    * @param count The number of threads to use
    */
    void SetThreadCount(const int count);

    /**
    * @brief Initialize the threads (connect all signals to resultsview's slots)
    *
    * @param view View to show error results
    */
    void Initialize(ResultsView *view);

    /**
    * @brief Load settings
    * @param settings QSettings to load settings from
    */
    void LoadSettings(QSettings &settings);

    /**
    * @brief Save settings
    * @param settings QSettings to save settings to
    */
    void SaveSettings(QSettings &settings);

    /**
    * @brief Clear all files from cppcheck
    *
    */
    void ClearFiles();

    /**
    * @brief Set files to check
    *
    * @param files files to check
    */
    void SetFiles(const QStringList &files);

    /**
    * @brief Start the threads to check the files
    *
    * @param settings Settings for checking
    * @param recheck Should we reuse the files we checked earleir
    */
    void Check(Settings settings, bool recheck);

    /**
    * @brief Is checking running?
    *
    * @return true if check is running, false otherwise.
    */
    bool IsChecking() const;

signals:
    /**
    * @brief Signal that all threads are done
    *
    */
    void Done();

public slots:

    /**
    * @brief Slot to stop all threads
    *
    */
    void Stop();
protected slots:



    /**
    * @brief Slot that a single thread is done
    *
    */
    void ThreadDone();
protected:
    /**
    * @brief List of files checked last time (used when rechecking)
    *
    */
    QStringList mLastFiles;

    /**
    * @brief Function to delete all threads
    *
    */
    void RemoveThreads();

    /**
    * @brief Thread results are stored here
    *
    */
    ThreadResult mResults;

    /**
    * @brief List of threads currently in use
    *
    */
    QList<CheckThread *> mThreads;

    /**
    * @brief The amount of threads currently running
    *
    */
    int mRunningThreadCount;
private:
};
/// @}
#endif // THREADHANDLER_H
