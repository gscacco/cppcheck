

=========
Cppcheck
=========


About

  The original name of this program is "C++check".
  The name was changed to "cppcheck".


Compiling

  Any C++ compiler should work.

  The Makefile works under Linux.
  To make it work under Windows with DJGPP, change "g++" to "gxx".

  To compile the GUI one needs Qt development libraries and issue the following commands:
  cd gui
  qmake
  lrelease gui.pro
  make

Usage

  Run the cppcheck program without parameters and a help text will be shown.


Recommendations

  When the "--all" flag is given you may get a lot of error messages.


Webpage

  http://www.sf.net/projects/cppcheck


