<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="en_US">
<context>
    <name>About</name>
    <message>
        <location filename="about.ui" line="14"/>
        <source>About Cppcheck</source>
        <translation>About Cppcheck</translation>
    </message>
    <message>
        <location filename="about.ui" line="51"/>
        <source>Version %1</source>
        <translation>Version %1</translation>
    </message>
    <message>
        <location filename="about.ui" line="58"/>
        <source>Cppcheck - A tool for static C/C++ code analysis.</source>
        <translation>Cppcheck - A tool for static C/C++ code analysis.</translation>
    </message>
    <message>
        <location filename="about.ui" line="65"/>
        <source>Copyright (C) 2007-2009 Daniel Marjamäki and cppcheck team.</source>
        <translation>Copyright (C) 2007-2009 Daniel Marjamäki and cppcheck team.</translation>
    </message>
    <message>
        <location filename="about.ui" line="72"/>
        <source>This program is licensed under the terms
of the GNU General Public License version 3</source>
        <translation>This program is licensed under the terms
of the GNU General Public License version 3</translation>
    </message>
    <message>
        <location filename="about.ui" line="83"/>
        <source>Visit Cppcheck homepage at %1</source>
        <translation>Visit Cppcheck homepage at %1</translation>
    </message>
</context>
<context>
    <name>ApplicationDialog</name>
    <message>
        <location filename="application.ui" line="14"/>
        <source>Add an application</source>
        <translation>Add a new application</translation>
    </message>
    <message>
        <location filename="application.ui" line="20"/>
        <source>Here you can add applications that can open error files.
Specify a name for the application and the application to execute.

The following texts are replaced with appriproate values when application is executed:
(file) - Filename containing the error
(line) - Line number containing the error
(message) - Error message
(severity) - Error severity

Example opening a file with Kate and make Kate scroll to the correct line:
kate -l(line) (file)</source>
        <translation>Here you can add applications that can open error files.
Specify a name for the application and the application to execute.

The following texts are replaced with appriproate values when application is executed:
(file) - Filename containing the error
(line) - Line number containing the error
(message) - Error message
(severity) - Error severity

Example opening a file with Kate and make Kate scroll to the correct line:
kate -l(line) (file)</translation>
    </message>
    <message>
        <location filename="application.ui" line="39"/>
        <source>Application&apos;s name</source>
        <translation>Application&apos;s name</translation>
    </message>
    <message>
        <location filename="application.ui" line="46"/>
        <source>Application to execute</source>
        <translation>Application to execute</translation>
    </message>
    <message>
        <location filename="application.ui" line="59"/>
        <source>Browse</source>
        <translation>Browse</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="57"/>
        <source>Executable files (*.exe);;All files(*.*)</source>
        <translation>Executable files (*.exe);;All files(*.*)</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="60"/>
        <source>Select viewer application</source>
        <translation>Select viewer application</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="97"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="98"/>
        <source>You must specify a name and a path for the application!</source>
        <translation>You must specify a name and a path for the application!</translation>
    </message>
</context>
<context>
    <name>FileViewDialog</name>
    <message>
        <location filename="fileviewdialog.cpp" line="37"/>
        <source>Close</source>
        <translation>Close</translation>
    </message>
    <message>
        <location filename="fileviewdialog.cpp" line="57"/>
        <source>Could not find the file: %1</source>
        <oldsource>Could not find the file:
</oldsource>
        <translation>Could not find the file: %1</translation>
    </message>
    <message>
        <location filename="fileviewdialog.cpp" line="61"/>
        <location filename="fileviewdialog.cpp" line="76"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="fileviewdialog.cpp" line="72"/>
        <source>Could not read the file: %1</source>
        <translation>Could not read the file: %1</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="main.ui" line="26"/>
        <location filename="mainwindow.cpp" line="243"/>
        <location filename="mainwindow.cpp" line="450"/>
        <location filename="mainwindow.cpp" line="548"/>
        <location filename="mainwindow.cpp" line="566"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="main.ui" line="253"/>
        <source>Show possible false positives</source>
        <translation>Show possible false positives</translation>
    </message>
    <message>
        <location filename="main.ui" line="74"/>
        <source>&amp;File</source>
        <translation>&amp;File</translation>
    </message>
    <message>
        <location filename="main.ui" line="81"/>
        <source>&amp;View</source>
        <translation>&amp;View</translation>
    </message>
    <message>
        <location filename="main.ui" line="110"/>
        <source>&amp;Check</source>
        <translation>&amp;Check</translation>
    </message>
    <message>
        <location filename="main.ui" line="119"/>
        <source>&amp;Edit</source>
        <translation>&amp;Edit</translation>
    </message>
    <message>
        <location filename="main.ui" line="134"/>
        <source>toolBar</source>
        <translation>toolBar</translation>
    </message>
    <message>
        <location filename="main.ui" line="152"/>
        <source>&amp;License...</source>
        <translation>&amp;License...</translation>
    </message>
    <message>
        <location filename="main.ui" line="157"/>
        <source>A&amp;uthors...</source>
        <translation>A&amp;uthors...</translation>
    </message>
    <message>
        <location filename="main.ui" line="166"/>
        <source>&amp;About...</source>
        <translation>&amp;About...</translation>
    </message>
    <message>
        <location filename="main.ui" line="171"/>
        <source>&amp;Files...</source>
        <translation>&amp;Files...</translation>
    </message>
    <message>
        <location filename="main.ui" line="174"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location filename="main.ui" line="183"/>
        <source>&amp;Directory...</source>
        <translation>&amp;Directory...</translation>
    </message>
    <message>
        <location filename="main.ui" line="186"/>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <location filename="main.ui" line="195"/>
        <source>&amp;Recheck files</source>
        <translation>&amp;Recheck files</translation>
    </message>
    <message>
        <location filename="main.ui" line="198"/>
        <source>Ctrl+R</source>
        <translation>Ctrl+R</translation>
    </message>
    <message>
        <location filename="main.ui" line="207"/>
        <source>&amp;Stop</source>
        <translation>&amp;Stop</translation>
    </message>
    <message>
        <location filename="main.ui" line="210"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="main.ui" line="219"/>
        <source>&amp;Save results to file...</source>
        <translation>&amp;Save results to file...</translation>
    </message>
    <message>
        <location filename="main.ui" line="222"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="main.ui" line="227"/>
        <source>&amp;Quit</source>
        <translation>&amp;Quit</translation>
    </message>
    <message>
        <location filename="main.ui" line="236"/>
        <source>&amp;Clear results</source>
        <translation>&amp;Clear results</translation>
    </message>
    <message>
        <location filename="main.ui" line="245"/>
        <source>&amp;Preferences</source>
        <translation>&amp;Preferences</translation>
    </message>
    <message>
        <location filename="main.ui" line="261"/>
        <source>Show security errors</source>
        <translation>Show security errors</translation>
    </message>
    <message>
        <location filename="main.ui" line="269"/>
        <source>Show style errors</source>
        <translation>Show style errors</translation>
    </message>
    <message>
        <location filename="main.ui" line="277"/>
        <source>Show common errors</source>
        <translation>Show common errors</translation>
    </message>
    <message>
        <location filename="main.ui" line="282"/>
        <source>&amp;Check all</source>
        <translation>&amp;Check all</translation>
    </message>
    <message>
        <location filename="main.ui" line="287"/>
        <source>&amp;Uncheck all</source>
        <translation>&amp;Uncheck all</translation>
    </message>
    <message>
        <location filename="main.ui" line="292"/>
        <source>Collapse &amp;all</source>
        <translation>Collapse &amp;all</translation>
    </message>
    <message>
        <location filename="main.ui" line="297"/>
        <source>&amp;Expand all</source>
        <translation>&amp;Expand all</translation>
    </message>
    <message>
        <location filename="main.ui" line="305"/>
        <source>&amp;Toolbar</source>
        <translation>&amp;Toolbar</translation>
    </message>
    <message>
        <location filename="main.ui" line="97"/>
        <source>&amp;Language</source>
        <translation>&amp;Language</translation>
    </message>
    <message>
        <location filename="main.ui" line="102"/>
        <source>&amp;Help</source>
        <translation>&amp;Help</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="206"/>
        <source>Select files to check</source>
        <translation>Select files to check</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="215"/>
        <source>Select directory to check</source>
        <translation>Select directory to check</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="244"/>
        <source>No suitable files found to check!</source>
        <translation>No suitable files found to check!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="446"/>
        <source>Cannot exit while checking.

Stop the checking before exiting.</source>
        <translation>Cannot exit while checking.

Stop the checking before exiting.</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="507"/>
        <source>XML files (*.xml);;Text files (*.txt)</source>
        <translation>XML files (*.xml);;Text files (*.txt)</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="509"/>
        <source>Save the report file</source>
        <translation>Save the report file</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="517"/>
        <source>XML files (*.xml)</source>
        <translation>XML files (*.xml)</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="550"/>
        <source>Cppcheck - %1</source>
        <translation>Cppcheck - %1</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="567"/>
        <source>Failed to change language:

%1</source>
        <translation>Failed to change language:

%1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="translationhandler.cpp" line="12"/>
        <source>English</source>
        <translation>English</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="13"/>
        <source>Finnish</source>
        <translation>Finnish</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="14"/>
        <source>Swedish</source>
        <translation>Swedish</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="15"/>
        <source>German</source>
        <translation>German</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="16"/>
        <source>Russian</source>
        <translation>Russian</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="69"/>
        <source>Incorrect language specified!</source>
        <translation>Incorrect language specified!</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="77"/>
        <source>Failed to load language from file %1</source>
        <translation>Failed to load language from file %1</translation>
    </message>
</context>
<context>
    <name>ResultsTree</name>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="754"/>
        <source>File</source>
        <translation>File</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="754"/>
        <source>Severity</source>
        <translation>Severity</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="754"/>
        <source>Line</source>
        <translation>Line</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="754"/>
        <source>Message</source>
        <translation>Message</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="85"/>
        <source>Undefined file</source>
        <translation>Undefined file</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="388"/>
        <source>Copy filename</source>
        <translation>Copy filename</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="389"/>
        <source>Copy full path</source>
        <translation>Copy full path</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="425"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="426"/>
        <source>You can open this error by specifying applications in program&apos;s settings.</source>
        <translation>You can open this error by specifying applications in program&apos;s settings.</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="471"/>
        <source>Could not start %1

Please check the application path and parameters are correct.</source>
        <translation>Could not start %1

Please check the application path and parameters are correct.</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="612"/>
        <source>all</source>
        <translation>All</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="616"/>
        <source>style</source>
        <translation>Style</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="620"/>
        <source>security</source>
        <translation>Security</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="624"/>
        <source>error</source>
        <translation>Error</translation>
    </message>
</context>
<context>
    <name>ResultsView</name>
    <message>
        <location filename="resultsview.cpp" line="77"/>
        <location filename="resultsview.cpp" line="89"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="78"/>
        <source>No errors found.</source>
        <translation>No errors found.</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="86"/>
        <source>Errors were found, but they are configured to be hidden.
To toggle what kind of errors are shown, open view menu.</source>
        <translation>Errors were found, but they are configured to be hidden.
To toggle what kind of errors are shown, open view menu.</translation>
    </message>
    <message>
        <location filename="resultsview.ui" line="26"/>
        <source>Results</source>
        <translation>Results</translation>
    </message>
</context>
<context>
    <name>Settings</name>
    <message>
        <location filename="settings.ui" line="14"/>
        <source>Settings</source>
        <translation>Settings</translation>
    </message>
    <message>
        <location filename="settings.ui" line="24"/>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <location filename="settings.ui" line="34"/>
        <source>Number of threads: </source>
        <translation>Number of threads: </translation>
    </message>
    <message>
        <location filename="settings.ui" line="46"/>
        <source>Check all #ifdef configurations</source>
        <translation>Check all #ifdef configurations</translation>
    </message>
    <message>
        <location filename="settings.ui" line="53"/>
        <source>Show full path of files</source>
        <translation>Show full path of files</translation>
    </message>
    <message>
        <location filename="settings.ui" line="60"/>
        <source>Show &quot;No errors found&quot; message when no errors found</source>
        <translation>Show &quot;No errors found&quot; message when no errors found</translation>
    </message>
    <message>
        <location filename="settings.ui" line="83"/>
        <source>Applications</source>
        <translation>Applications</translation>
    </message>
    <message>
        <location filename="settings.ui" line="92"/>
        <source>Add application</source>
        <translation>Add application</translation>
    </message>
    <message>
        <location filename="settings.ui" line="99"/>
        <source>Delete application</source>
        <translation>Delete application</translation>
    </message>
    <message>
        <location filename="settings.ui" line="106"/>
        <source>Modify application</source>
        <translation>Modify application</translation>
    </message>
    <message>
        <location filename="settings.ui" line="113"/>
        <source>Set as default application</source>
        <translation>Set as default application</translation>
    </message>
    <message>
        <location filename="settings.ui" line="121"/>
        <source>Reports</source>
        <translation>Reports</translation>
    </message>
    <message>
        <location filename="settings.ui" line="127"/>
        <source>Save all errors when creating report</source>
        <translation>Save all errors when creating report</translation>
    </message>
    <message>
        <location filename="settings.ui" line="134"/>
        <source>Save full path to files in reports</source>
        <translation>Save full path to files in reports</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="settingsdialog.cpp" line="132"/>
        <source>Add a new application</source>
        <translation>Add a new application</translation>
    </message>
    <message>
        <location filename="settingsdialog.cpp" line="165"/>
        <source>Modify an application</source>
        <translation>Modify an application</translation>
    </message>
</context>
</TS>