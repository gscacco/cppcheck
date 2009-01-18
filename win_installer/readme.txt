Windows installer for the cppcheck
----------------------------------

Simple Windows installer installing executable, license file and readme.txt to
program files/cppcheck folder.

Shortcuts are created to start cmd.exe in installation folder. So when the user
selects start menu/desktop icon he gets command prompt in cppcheck folder.

Get the InnoSetup from:
http://www.innosetup.com/
Be sure to download the 'QuickStart Pack' as it installs some nice tools
like ISTool and preprocessor support.

Files the installer needs:
/Release/cppcheck.exe
/COPYING
/readme.txt

Creating the installer executable:
#1 Open the ISTool and load cppcheck.iss
#2 Update the release version number:
   - look for line "#define AppVersion"
#3 Check all files are present:
   - from menu: Project / Verify files...
#4 Compile the installer
   - from menu: Project / Compile Setup

If compilation succeeds, the installer executable is created into folder
/win_installer/output. The filename is cppcheck-[version]-setup.exe.