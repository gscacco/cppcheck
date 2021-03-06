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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileDialog>
#include <QSignalMapper>
#include <QActionGroup>
#include <QToolBar>

#include "resultsview.h"
#include "settingsdialog.h"
#include "translationhandler.h"

#include "ui_main.h"
class ThreadHandler;


/// @addtogroup GUI
/// @{


/**
* @brief Main window for cppcheck-gui
*
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

public slots:



    /**
    * @brief Slot for check files menu item
    *
    */
    void CheckFiles();

    /**
    * @brief Slot to recheck files
    *
    */
    void ReCheck();

    /**
    * @brief Slot to clear all search results
    *
    */
    void ClearResults();

    /**
    * @brief Show errors with type "possible error"
    * @param checked Should errors be shown (true) or hidden (false)
    */
    void ShowAll(bool checked);

    /**
    * @brief Show errors with type "style"
    * @param checked Should errors be shown (true) or hidden (false)
    */
    void ShowStyle(bool checked);

    /**
    * @brief Show errors with type "possible style"
    * @param checked Should errors be shown (true) or hidden (false)
    */
    void ShowAllStyle(bool checked);

    /**
    * @brief Show errors with type "error"
    * @param checked Should errors be shown (true) or hidden (false)
    */
    void ShowErrors(bool checked);

    /**
    * @brief Slot to check all "Show errors" menu items
    */
    void CheckAll();

    /**
    * @brief Slot to uncheck all "Show errors" menu items
    */
    void UncheckAll();

    /**
    * @brief Slot for check directory menu item
    *
    */
    void CheckDirectory();

    /**
    * @brief Slot to open program's settings dialog
    *
    */
    void ProgramSettings();

    /**
    * @brief Slot to open program's about dialog
    *
    */
    void About();

    /**
    * @brief Slot to to show license text
    *
    */
    void ShowLicense();

    /**
    * @brief Slot to to show authors list
    *
    */
    void ShowAuthors();

    /**
    * @brief Slot to stop processing files
    *
    */
    void Save();

protected slots:

    /**
    * @brief Slot for checkthread's done signal
    *
    */
    void CheckDone();

    /**
    * @brief Slot for enabling save and clear button
    *
    */
    void ResultsAdded();


    /**
    * @brief Slot for changing the program's language
    *
    */
    void MapLanguage(QAction *);

    /**
    * @brief Slot for showing/hiding standard toolbar
    */
    void ToggleToolbar();

    /**
    * @brief Slot for updating View-menu before it is shown.
    */
    void AboutToShowViewMenu();

    /**
    * @brief Slot when stop checking button is pressed
    *
    */
    void StopChecking();

protected:

    /**
    * @brief Create menu items to change language
    *
    */
    void CreateLanguageMenuItems();

    /**
    * @brief Set current language
    * @param index Index of the language to set
    */
    void SetLanguage(const int index);

    /**
    * @brief Event coming when application is about to close.
    */
    virtual void closeEvent(QCloseEvent *event);

    /**
    * @brief Helper function to toggle all show error menu items
    * @param checked Should all errors be shown (true) or hidden (false)
    */
    void ToggleAllChecked(bool checked);

    /**
    * @brief Helper function to enable/disable all check,recheck buttons
    *
    */
    void EnableCheckButtons(bool enable);

    /**
    * @brief Select files/or directory to check.
    * Helper function to open a dialog to ask user to select files or
    * directory to check. Use native dialogs instead of QT:s own dialogs.
    *
    * @param mode Dialog open mode (files or directories)
    * @return QStringList of files or directories that were selected to check
    */
    QStringList SelectFilesToCheck(QFileDialog::FileMode mode);

    /**
    * @brief Check all files specified in parameter files
    *
    * @param files List of files and/or directories to check
    */
    void DoCheckFiles(const QStringList &files);

    /**
    * @brief Get all files recursively from given path
    *
    * @param path Path to get files from
    * @return List of file paths
    */
    QStringList GetFilesRecursively(const QString &path);

    /**
    * @brief Get our default cppcheck settings and read project file.
    *
    * @return Default cppcheck settings
    */
    Settings GetCppcheckSettings();

    /**
    * @brief Removes all unaccepted (by cppcheck core) files from the list
    *
    * @param list List to remove unaccepted files from
    * @return List of files that are all accepted by cppcheck core
    */
    QStringList RemoveUnacceptedFiles(const QStringList &list);

    /**
    * @brief Load program settings
    *
    */
    void LoadSettings();

    /**
    * @brief Save program settings
    *
    */
    void SaveSettings();

    /**
    * @brief Format main window title.
    * @param text Text added to end of the title.
    */
    void FormatAndSetTitle(const QString &text = QString());

    /**
    * @brief Program settings
    *
    */
    QSettings *mSettings;




    /**
    * @brief Results for checking
    *
    */
//    ResultsView mResults;

    /**
    * @brief Thread to check files
    *
    */
    ThreadHandler *mThread;

    /**
    * @brief List of user defined applications to open errors with
    *
    */
    ApplicationList *mApplications;

    /**
    * @brief Class to handle translation changes
    *
    */
    TranslationHandler *mTranslation;

    /**
    * @brief Class holding all UI components
    *
    */
    Ui::MainWindow mUI;

    /**
    * @brief Group holding all supported languages
    *
    */
    QActionGroup *mLanguages;

    /**
    * @brief Current checked directory.
    */
    QString mCurrentDirectory;

};
/// @}
#endif // MAINWINDOW_H
