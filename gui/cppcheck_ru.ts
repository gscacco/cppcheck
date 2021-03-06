<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>About</name>
    <message>
        <location filename="about.ui" line="14"/>
        <source>About Cppcheck</source>
        <translation>О Cppcheck</translation>
    </message>
    <message>
        <location filename="about.ui" line="64"/>
        <source>Version %1</source>
        <translation>Версия %1</translation>
    </message>
    <message>
        <location filename="about.ui" line="71"/>
        <source>Cppcheck - A tool for static C/C++ code analysis.</source>
        <translation>Cppcheck - программа для статического анализа кода на языках С/С++</translation>
    </message>
    <message>
        <location filename="about.ui" line="81"/>
        <source>Copyright (C) 2007-2009 Daniel Marjamäki and cppcheck team.</source>
        <oldsource>Copyright (C) 2007-2009 Daniel Marjam??ki and cppcheck team.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="about.ui" line="91"/>
        <source>This program is licensed under the terms
of the GNU General Public License version 3</source>
        <translation>Эта программа распространяется на
условиях лицензии GNU General Public License, версии 3</translation>
    </message>
    <message>
        <location filename="about.ui" line="102"/>
        <source>Visit Cppcheck homepage at %1</source>
        <translation>Посетите домашнюю страницу: %1</translation>
    </message>
</context>
<context>
    <name>ApplicationDialog</name>
    <message>
        <location filename="application.ui" line="14"/>
        <source>Add an application</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="application.ui" line="39"/>
        <source>Application&apos;s name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="application.ui" line="46"/>
        <source>Application to execute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="application.ui" line="59"/>
        <source>Browse</source>
        <translation>Просмотреть</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="57"/>
        <source>Executable files (*.exe);;All files(*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="60"/>
        <source>Select viewer application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="97"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="98"/>
        <source>You must specify a name and a path for the application!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FileViewDialog</name>
    <message>
        <location filename="fileviewdialog.cpp" line="47"/>
        <source>Could not find the file: %1</source>
        <oldsource>Could not find the file:
</oldsource>
        <translation>Невозможно найти файл: %1</translation>
    </message>
    <message>
        <location filename="fileviewdialog.cpp" line="51"/>
        <location filename="fileviewdialog.cpp" line="66"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="fileviewdialog.cpp" line="62"/>
        <source>Could not read the file: %1</source>
        <translation>Невозможно прочитать файл: %1</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="main.ui" line="26"/>
        <location filename="mainwindow.cpp" line="220"/>
        <location filename="mainwindow.cpp" line="471"/>
        <location filename="mainwindow.cpp" line="574"/>
        <location filename="mainwindow.cpp" line="592"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="main.ui" line="136"/>
        <source>Standard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.ui" line="255"/>
        <source>Show possible false positives</source>
        <translation>Показывать возможные ложные срабатывания</translation>
    </message>
    <message>
        <location filename="main.ui" line="77"/>
        <source>&amp;File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="main.ui" line="84"/>
        <source>&amp;View</source>
        <translation>Вид</translation>
    </message>
    <message>
        <location filename="main.ui" line="113"/>
        <source>&amp;Check</source>
        <translation>Проверить</translation>
    </message>
    <message>
        <location filename="main.ui" line="122"/>
        <source>&amp;Edit</source>
        <translation>Правка</translation>
    </message>
    <message>
        <location filename="main.ui" line="154"/>
        <source>&amp;License...</source>
        <translation>Лицензия...</translation>
    </message>
    <message>
        <location filename="main.ui" line="159"/>
        <source>A&amp;uthors...</source>
        <translation>Авторы...</translation>
    </message>
    <message>
        <location filename="main.ui" line="168"/>
        <source>&amp;About...</source>
        <translation>О программе...</translation>
    </message>
    <message>
        <location filename="main.ui" line="173"/>
        <source>&amp;Files...</source>
        <translation>Файлы...</translation>
    </message>
    <message>
        <location filename="main.ui" line="176"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location filename="main.ui" line="185"/>
        <source>&amp;Directory...</source>
        <translation>Каталог...</translation>
    </message>
    <message>
        <location filename="main.ui" line="188"/>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <location filename="main.ui" line="197"/>
        <source>&amp;Recheck files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.ui" line="200"/>
        <source>Ctrl+R</source>
        <translation>Ctrl+R</translation>
    </message>
    <message>
        <location filename="main.ui" line="209"/>
        <source>&amp;Stop</source>
        <translation>Остановить</translation>
    </message>
    <message>
        <location filename="main.ui" line="212"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="main.ui" line="221"/>
        <source>&amp;Save results to file...</source>
        <translation>Сохранить отчёт в файл...</translation>
    </message>
    <message>
        <location filename="main.ui" line="224"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="main.ui" line="229"/>
        <source>&amp;Quit</source>
        <translation>Выход</translation>
    </message>
    <message>
        <location filename="main.ui" line="238"/>
        <source>&amp;Clear results</source>
        <translation>Очистить отчёт</translation>
    </message>
    <message>
        <location filename="main.ui" line="247"/>
        <source>&amp;Preferences</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="main.ui" line="263"/>
        <source>Show security errors</source>
        <translation>Показывать ошибки безопасности</translation>
    </message>
    <message>
        <location filename="main.ui" line="271"/>
        <source>Show style errors</source>
        <translation>Показывать ошибки стиля</translation>
    </message>
    <message>
        <location filename="main.ui" line="279"/>
        <source>Show possible style errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="main.ui" line="287"/>
        <source>Show common errors</source>
        <translation>Показывать общие ошибки</translation>
    </message>
    <message>
        <location filename="main.ui" line="292"/>
        <source>&amp;Check all</source>
        <translation>Отметить все</translation>
    </message>
    <message>
        <location filename="main.ui" line="297"/>
        <source>&amp;Uncheck all</source>
        <translation>Сбросить все</translation>
    </message>
    <message>
        <location filename="main.ui" line="302"/>
        <source>Collapse &amp;all</source>
        <translation>Свернуть все</translation>
    </message>
    <message>
        <location filename="main.ui" line="307"/>
        <source>&amp;Expand all</source>
        <translation>Развернуть все</translation>
    </message>
    <message>
        <location filename="main.ui" line="315"/>
        <source>&amp;Toolbar</source>
        <translation>Панель инструментов</translation>
    </message>
    <message>
        <location filename="main.ui" line="100"/>
        <source>&amp;Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="main.ui" line="105"/>
        <source>&amp;Help</source>
        <translation>Помощь</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="254"/>
        <source>Select files to check</source>
        <translation>Выберите файлы для проверки</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="263"/>
        <source>Select directory to check</source>
        <translation>Выберите каталог для проверки</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="221"/>
        <source>No suitable files found to check!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="467"/>
        <source>Cannot exit while checking.

Stop the checking before exiting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="513"/>
        <source>License</source>
        <translation>Лицензия</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="520"/>
        <source>Authors</source>
        <translation>Авторы</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="528"/>
        <source>XML files (*.xml);;Text files (*.txt);;CSV files (*.csv)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="530"/>
        <source>Save the report file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="538"/>
        <source>XML files (*.xml)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="544"/>
        <source>Text files (*.txt)</source>
        <translation>Текстовые файлы (*.txt)</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="576"/>
        <source>Cppcheck - %1</source>
        <translation>Cppcheck - %1</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="593"/>
        <source>Failed to change the language:

%1

</source>
        <oldsource>Failed to change language:

%1</oldsource>
        <translation>Невозможно изменить язык приложения:

%1

</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="32"/>
        <source>Finnish</source>
        <translation>Финский</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="31"/>
        <source>English</source>
        <translation>Английский</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="33"/>
        <source>Swedish</source>
        <translation>Швецкий</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="34"/>
        <source>German</source>
        <translation>Немецкий</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="35"/>
        <source>Russian</source>
        <translation>Русский</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="36"/>
        <source>Polish</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="translationhandler.cpp" line="90"/>
        <source>Incorrect language specified!</source>
        <translation>Выбран неверный язык!</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="100"/>
        <source>Language file %1 not found!</source>
        <oldsource>Language file %1.qm not found!</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="106"/>
        <source>Failed to load translation for language %1 from file %2</source>
        <oldsource>Failed to load translation for language %1 from file %2.qm</oldsource>
        <translation>Ошибка загрузки переводов для языка %1 из файла %2</translation>
    </message>
</context>
<context>
    <name>ResultsTree</name>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>Severity</source>
        <translation>Важность</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>Line</source>
        <translation>Строка</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>Message</source>
        <translation>Сообщение</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="85"/>
        <source>Undefined file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="388"/>
        <source>Copy filename</source>
        <translation>Скопировать имя файла</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="389"/>
        <source>Copy full path</source>
        <translation>Скопировать полный путь</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="425"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="426"/>
        <source>Configure the text file viewer program in Cppcheck preferences/Applications.</source>
        <oldsource>You can open this error by specifying applications in program&apos;s settings.</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="471"/>
        <source>Could not start %1

Please check the application path and parameters are correct.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="613"/>
        <source>possible error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="617"/>
        <source>style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="621"/>
        <source>possible style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="625"/>
        <source>error</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ResultsView</name>
    <message>
        <location filename="resultsview.cpp" line="79"/>
        <location filename="resultsview.cpp" line="91"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="80"/>
        <source>No errors found.</source>
        <translation>Ошибок не найдено.</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="88"/>
        <source>Errors were found, but they are configured to be hidden.
To toggle what kind of errors are shown, open view menu.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="139"/>
        <source>No errors found, nothing to save.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="166"/>
        <location filename="resultsview.cpp" line="176"/>
        <source>Failed to save the report.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="resultsview.ui" line="26"/>
        <source>Results</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Settings</name>
    <message>
        <location filename="settings.ui" line="14"/>
        <source>Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settings.ui" line="24"/>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <location filename="settings.ui" line="34"/>
        <source>Number of threads: </source>
        <translation>Количество потоков исполнения:</translation>
    </message>
    <message>
        <location filename="settings.ui" line="46"/>
        <source>Check all #ifdef configurations</source>
        <translation>Проверять все варианты #ifdef конфигураций</translation>
    </message>
    <message>
        <location filename="settings.ui" line="53"/>
        <source>Show full path of files</source>
        <translation>Показывать полные пути к файлам</translation>
    </message>
    <message>
        <location filename="settings.ui" line="60"/>
        <source>Show &quot;No errors found&quot; message when no errors found</source>
        <translation>Показывать сообщение, если ошибок не найдено</translation>
    </message>
    <message>
        <location filename="settings.ui" line="83"/>
        <source>Applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settings.ui" line="92"/>
        <source>Add application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settings.ui" line="99"/>
        <source>Delete application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settings.ui" line="106"/>
        <source>Modify application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settings.ui" line="113"/>
        <source>Set as default application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settings.ui" line="121"/>
        <source>Reports</source>
        <translation>Отчёты</translation>
    </message>
    <message>
        <location filename="settings.ui" line="127"/>
        <source>Save all errors when creating report</source>
        <translation>Сохранять все ошибки при создании отчёта</translation>
    </message>
    <message>
        <location filename="settings.ui" line="134"/>
        <source>Save full path to files in reports</source>
        <translation>Сохранять полные пути к файлам в отчётах</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="settingsdialog.cpp" line="132"/>
        <source>Add a new application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="settingsdialog.cpp" line="165"/>
        <source>Modify an application</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
