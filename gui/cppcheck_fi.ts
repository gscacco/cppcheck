<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="fi_FI">
<context>
    <name>About</name>
    <message>
        <location filename="about.ui" line="14"/>
        <source>About Cppcheck</source>
        <translation>Tietoa ohjelmasta Cppcheck</translation>
    </message>
    <message>
        <location filename="about.ui" line="64"/>
        <source>Version %1</source>
        <translation>Versio %1</translation>
    </message>
    <message>
        <location filename="about.ui" line="71"/>
        <source>Cppcheck - A tool for static C/C++ code analysis.</source>
        <translation>Cppcheck - Työkalu C/C++ koodin staattiseen analysointiin.</translation>
    </message>
    <message>
        <location filename="about.ui" line="81"/>
        <source>Copyright (C) 2007-2009 Daniel Marjamäki and cppcheck team.</source>
        <translation>Copyright (C) 2007-2009 Daniel Marjamäki ja cppcheck tiimi.</translation>
    </message>
    <message>
        <location filename="about.ui" line="91"/>
        <source>This program is licensed under the terms
of the GNU General Public License version 3</source>
        <translation>Tämä ohjelma on lisensoitu GNU General 
Public lisenssin version 3 alaisuuteen</translation>
    </message>
    <message>
        <location filename="about.ui" line="102"/>
        <source>Visit Cppcheck homepage at %1</source>
        <translation>Cppcheckin kotisivu löytyy osoitteesta %1</translation>
    </message>
</context>
<context>
    <name>ApplicationDialog</name>
    <message>
        <location filename="application.ui" line="14"/>
        <source>Add an application</source>
        <translation>Lisää uusi ohjelma</translation>
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
        <translation>Voi lisätä ohjelmia joilla avata virheen sisältämiä tiedostoja
määrittämällä suoritettavan ohjelman ja nimen ohjelmalle.

Suoritettavalle ohjelmalle voi määrittää parametreja jotka Cppcheck korvaa sopivilla arvoilla.
Parametrit ovat seuraavat:
(file) - Tiedosto joka sisältää virheen
(line) - Rivinumero josta virhe löytyy
(message) - Virheilmoitus
(severity) - Virheen vakavuus

Esimerkki virheen avaamisesta Kate nimisellä tekstinkäsittelyohjelmalla:
kate -l(line) (file)
</translation>
    </message>
    <message>
        <location filename="application.ui" line="39"/>
        <source>Application&apos;s name</source>
        <translation>Ohjelman nimi</translation>
    </message>
    <message>
        <location filename="application.ui" line="46"/>
        <source>Application to execute</source>
        <translation>Suoritettava ohjelma</translation>
    </message>
    <message>
        <location filename="application.ui" line="59"/>
        <source>Browse</source>
        <translation>Selaa</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="57"/>
        <source>Executable files (*.exe);;All files(*.*)</source>
        <translation>Suoritettavat tiedostot (*.exe);;Kaikki tiedostot(*.*)</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="60"/>
        <source>Select viewer application</source>
        <translation>Valitse ohjelma jolla avata virhetiedosto</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="97"/>
        <source>Cppcheck</source>
        <translation>Cppcheck</translation>
    </message>
    <message>
        <location filename="applicationdialog.cpp" line="98"/>
        <source>You must specify a name and a path for the application!</source>
        <translation>Suoritettava ohjelma tulee määrittää!</translation>
    </message>
</context>
<context>
    <name>FileViewDialog</name>
    <message>
        <location filename="fileviewdialog.cpp" line="47"/>
        <source>Could not find the file: %1</source>
        <oldsource>Could not find the file:
</oldsource>
        <translation>Tiedostoa %1 ei löytynyt</translation>
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
        <translation>Tiedoston %1 lukeminen epäonnistui</translation>
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
        <translation>Vakio</translation>
    </message>
    <message>
        <location filename="main.ui" line="255"/>
        <source>Show possible false positives</source>
        <translation>Näytä epävarmat virheet</translation>
    </message>
    <message>
        <location filename="main.ui" line="77"/>
        <source>&amp;File</source>
        <translation>&amp;Tiedosto</translation>
    </message>
    <message>
        <location filename="main.ui" line="84"/>
        <source>&amp;View</source>
        <translation>&amp;Näytä</translation>
    </message>
    <message>
        <location filename="main.ui" line="113"/>
        <source>&amp;Check</source>
        <translation>&amp;Tarkista</translation>
    </message>
    <message>
        <location filename="main.ui" line="122"/>
        <source>&amp;Edit</source>
        <translation>&amp;Muokkaa</translation>
    </message>
    <message>
        <location filename="main.ui" line="154"/>
        <source>&amp;License...</source>
        <translation>&amp;Lisenssi...</translation>
    </message>
    <message>
        <location filename="main.ui" line="159"/>
        <source>A&amp;uthors...</source>
        <translation>&amp;Tekijät...</translation>
    </message>
    <message>
        <location filename="main.ui" line="168"/>
        <source>&amp;About...</source>
        <translation>&amp;Tietoa ohjelmasta Cppcheck...</translation>
    </message>
    <message>
        <location filename="main.ui" line="173"/>
        <source>&amp;Files...</source>
        <translation>&amp;Tiedostot...</translation>
    </message>
    <message>
        <location filename="main.ui" line="176"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location filename="main.ui" line="185"/>
        <source>&amp;Directory...</source>
        <translation>&amp;Hakemisto...</translation>
    </message>
    <message>
        <location filename="main.ui" line="188"/>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <location filename="main.ui" line="197"/>
        <source>&amp;Recheck files</source>
        <translation>Tarkista tiedostot &amp;uudelleen</translation>
    </message>
    <message>
        <location filename="main.ui" line="200"/>
        <source>Ctrl+R</source>
        <translation>Ctrl+R</translation>
    </message>
    <message>
        <location filename="main.ui" line="209"/>
        <source>&amp;Stop</source>
        <translation>&amp;Pysäytä</translation>
    </message>
    <message>
        <location filename="main.ui" line="212"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="main.ui" line="221"/>
        <source>&amp;Save results to file...</source>
        <translation>&amp;Tallenna tulokset tiedostoon...</translation>
    </message>
    <message>
        <location filename="main.ui" line="224"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="main.ui" line="229"/>
        <source>&amp;Quit</source>
        <translation>&amp;Lopeta</translation>
    </message>
    <message>
        <location filename="main.ui" line="238"/>
        <source>&amp;Clear results</source>
        <translation>&amp;Tyhjennä tulokset</translation>
    </message>
    <message>
        <location filename="main.ui" line="247"/>
        <source>&amp;Preferences</source>
        <translation>&amp;Asetukset</translation>
    </message>
    <message>
        <location filename="main.ui" line="263"/>
        <source>Show security errors</source>
        <translation>Näytä tietoturvavirheet</translation>
    </message>
    <message>
        <location filename="main.ui" line="271"/>
        <source>Show style errors</source>
        <translation>Näytä tyylivirheet</translation>
    </message>
    <message>
        <location filename="main.ui" line="279"/>
        <source>Show possible style errors</source>
        <translation>Näytä mahdolliset tyylivirheet</translation>
    </message>
    <message>
        <location filename="main.ui" line="287"/>
        <source>Show common errors</source>
        <translation>Näytä yleiset virheet</translation>
    </message>
    <message>
        <location filename="main.ui" line="292"/>
        <source>&amp;Check all</source>
        <translation>&amp;Valitse kaikki</translation>
    </message>
    <message>
        <location filename="main.ui" line="297"/>
        <source>&amp;Uncheck all</source>
        <translation>&amp;Poista kaikista valinta</translation>
    </message>
    <message>
        <location filename="main.ui" line="302"/>
        <source>Collapse &amp;all</source>
        <translation>&amp;Pienennä kaikki</translation>
    </message>
    <message>
        <location filename="main.ui" line="307"/>
        <source>&amp;Expand all</source>
        <translation>&amp;Laajenna kaikki</translation>
    </message>
    <message>
        <location filename="main.ui" line="315"/>
        <source>&amp;Toolbar</source>
        <translation>&amp;Työkalupalkki</translation>
    </message>
    <message>
        <location filename="main.ui" line="100"/>
        <source>&amp;Language</source>
        <translation>&amp;Kieli</translation>
    </message>
    <message>
        <location filename="main.ui" line="105"/>
        <source>&amp;Help</source>
        <translation>&amp;Ohje</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="254"/>
        <source>Select files to check</source>
        <translation>Valitse tarkistettavat tiedostot</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="263"/>
        <source>Select directory to check</source>
        <translation>Valitse tarkistettava hakemisto</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="221"/>
        <source>No suitable files found to check!</source>
        <translation>Tarkistettavaksi sopivia tiedostoja ei löytynyt!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="467"/>
        <source>Cannot exit while checking.

Stop the checking before exiting.</source>
        <translation>Ohjelmaa ei voi sammuttaa tarkistuksen ollessa käynnissä.

Lopeta tarkistus ennen ohjelman sammuttamista.</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="513"/>
        <source>License</source>
        <translation>Lisenssi</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="520"/>
        <source>Authors</source>
        <translation>Tekijät</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="528"/>
        <source>XML files (*.xml);;Text files (*.txt);;CSV files (*.csv)</source>
        <translation>XML-tiedostot (*.xml);;Tekstitiedostot (*.txt);;CSV-tiedostot (*.csv)</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="530"/>
        <source>Save the report file</source>
        <translation>Tallenna raportti</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="538"/>
        <source>XML files (*.xml)</source>
        <translation>XML-tiedostot (*xml)</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="544"/>
        <source>Text files (*.txt)</source>
        <translation>Tekstitiedostot (*.txt)</translation>
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
        <translation>Ohjelman kielen vaihtaminen epäonnistui:

%1

</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="32"/>
        <source>Finnish</source>
        <translation>Suomi</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="31"/>
        <source>English</source>
        <translation>Englanti</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="33"/>
        <source>Swedish</source>
        <translation>Ruotsi</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="34"/>
        <source>German</source>
        <translation>Saksa</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="35"/>
        <source>Russian</source>
        <translation>Venäjä</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="36"/>
        <source>Polish</source>
        <translation>Puola</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="translationhandler.cpp" line="90"/>
        <source>Incorrect language specified!</source>
        <translation>Virheellinen kieli valittu!</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="100"/>
        <source>Language file %1 not found!</source>
        <oldsource>Language file %1.qm not found!</oldsource>
        <translation>Käännöstiedostoa %1 ei löytynyt!</translation>
    </message>
    <message>
        <location filename="translationhandler.cpp" line="106"/>
        <source>Failed to load translation for language %1 from file %2</source>
        <oldsource>Failed to load translation for language %1 from file %2.qm</oldsource>
        <translation>Käänöksen lataaminen kielelle %1 tiedostosta %2 epäonnistui</translation>
    </message>
</context>
<context>
    <name>ResultsTree</name>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>File</source>
        <translation>Tiedosto</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>Severity</source>
        <translation>Tyyppi</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>Line</source>
        <translation>Rivi</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="39"/>
        <location filename="resultstree.cpp" line="755"/>
        <source>Message</source>
        <translation>Virhe</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="85"/>
        <source>Undefined file</source>
        <translation>Määrittelemätön tiedosto</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="388"/>
        <source>Copy filename</source>
        <translation>Kopioi tiedostonimi</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="389"/>
        <source>Copy full path</source>
        <translation>Kopioi tiedoston koko polku</translation>
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
        <translation>Voit asetuksista määritellä muita ohjelmia joilla avata tämän virheen sisältävän tiedoston.</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="471"/>
        <source>Could not start %1

Please check the application path and parameters are correct.</source>
        <translation>Ohjelman %1 käynnistäminen epäonnistui

Tarkista että ohjelman polku ja parametrit ovat oikeat.</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="613"/>
        <source>possible error</source>
        <translation>mahdollinen virhe</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="617"/>
        <source>style</source>
        <translation>Tyyli</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="621"/>
        <source>possible style</source>
        <translation>tyylivirhe</translation>
    </message>
    <message>
        <location filename="resultstree.cpp" line="625"/>
        <source>error</source>
        <translation>Yleinen</translation>
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
        <translation>Virheitä ei löytynyt.</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="88"/>
        <source>Errors were found, but they are configured to be hidden.
To toggle what kind of errors are shown, open view menu.</source>
        <translation>Virheitä löytyi, mutta asetuksissa kyseiset virheet on määritelty piilotettavaksi.
Määrittääksesi minkä tyyppisiä virheitä näytetään, avaa näkymä valikko.</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="139"/>
        <source>No errors found, nothing to save.</source>
        <translation>Virheitä ei löytynyt, ei mitään tallennettavaa.</translation>
    </message>
    <message>
        <location filename="resultsview.cpp" line="166"/>
        <location filename="resultsview.cpp" line="176"/>
        <source>Failed to save the report.</source>
        <translation>Raportin tallentaminen epäonnistui.</translation>
    </message>
    <message>
        <location filename="resultsview.ui" line="26"/>
        <source>Results</source>
        <translation>Tulokset</translation>
    </message>
</context>
<context>
    <name>Settings</name>
    <message>
        <location filename="settings.ui" line="14"/>
        <source>Preferences</source>
        <translation>Asetukset</translation>
    </message>
    <message>
        <location filename="settings.ui" line="24"/>
        <source>General</source>
        <translation>Yleiset</translation>
    </message>
    <message>
        <location filename="settings.ui" line="34"/>
        <source>Number of threads: </source>
        <translation>Säikeiden lukumäärä: </translation>
    </message>
    <message>
        <location filename="settings.ui" line="46"/>
        <source>Check all #ifdef configurations</source>
        <translation>Tarkista kaikki #ifdef kombinaatiot</translation>
    </message>
    <message>
        <location filename="settings.ui" line="53"/>
        <source>Show full path of files</source>
        <translation>Näytä tiedostojen täysi polku</translation>
    </message>
    <message>
        <location filename="settings.ui" line="60"/>
        <source>Show &quot;No errors found&quot; message when no errors found</source>
        <translation>Näytä &quot;virheitä ei löytynyt&quot;-viesti jos virheitä ei löydy</translation>
    </message>
    <message>
        <location filename="settings.ui" line="83"/>
        <source>Applications</source>
        <translation>Ohjelmat</translation>
    </message>
    <message>
        <location filename="settings.ui" line="92"/>
        <source>Add application</source>
        <translation>Lisää ohjelma</translation>
    </message>
    <message>
        <location filename="settings.ui" line="99"/>
        <source>Delete application</source>
        <translation>Poista ohjelma</translation>
    </message>
    <message>
        <location filename="settings.ui" line="106"/>
        <source>Modify application</source>
        <translation>Muokkaa ohjelmaa</translation>
    </message>
    <message>
        <location filename="settings.ui" line="113"/>
        <source>Set as default application</source>
        <translation>Aseta oletusohjelmaksi</translation>
    </message>
    <message>
        <location filename="settings.ui" line="121"/>
        <source>Reports</source>
        <translation>Raportit</translation>
    </message>
    <message>
        <location filename="settings.ui" line="127"/>
        <source>Save all errors when creating report</source>
        <translation>Tallenna kaikki virheet raporttia luodessa</translation>
    </message>
    <message>
        <location filename="settings.ui" line="134"/>
        <source>Save full path to files in reports</source>
        <translation>Tallenna tiedostojen koko polku raportteihin</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="settingsdialog.cpp" line="132"/>
        <source>Add a new application</source>
        <translation>Lisää uusi ohjelma</translation>
    </message>
    <message>
        <location filename="settingsdialog.cpp" line="165"/>
        <source>Modify an application</source>
        <translation>Muokkaa ohjelmaa</translation>
    </message>
</context>
</TS>
