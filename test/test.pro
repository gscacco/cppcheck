######################################################################
# Automatically generated by qmake (2.01a) Mon Jul 13 17:37:53 2009
######################################################################

TEMPLATE = app
TARGET = test
DEPENDPATH += .
INCLUDEPATH += ../src
OBJECTS_DIR = temp
CONFIG += warn_on debug
QT -= gui core
DEFINES += UNIT_TESTING

# Input
HEADERS += testsuite.h \
           ../src/tokenize.h \
           ../src/settings.h \
           ../src/errorlogger.h \
           ../src/token.h \
           ../src/checkautovariables.h \
           ../src/check.h \
           ../src/checkbufferoverrun.h \
           ../src/checkother.h \
           ../src/checkclass.h \
           ../src/cppcheck.h \
           ../src/checkunusedfunctions.h \
           ../src/checkdangerousfunctions.h \
           ../src/filelister.h \
           ../src/mathlib.h \
           ../src/checkmemoryleak.h \
           ../src/preprocessor.h \
           ../src/checkstl.h \
           ../src/checkheaders.h
SOURCES += testautovariables.cpp \
           testbufferoverrun.cpp \
           testcharvar.cpp \
           testclass.cpp \
           testconstructors.cpp \
           testcppcheck.cpp \
           testdangerousfunctions.cpp \
           testdivision.cpp \
           testfilelister.cpp \
           testincompletestatement.cpp \
           testmathlib.cpp \
           testmemleak.cpp \
           testother.cpp \
           testpreprocessor.cpp \
           testredundantif.cpp \
           testrunner.cpp \
           testsimplifytokens.cpp \
           teststl.cpp \
           testsuite.cpp \
           testtoken.cpp \
           testtokenize.cpp \
           testunusedfunctions.cpp \
           testunusedprivfunc.cpp \
           testunusedvar.cpp \
           ../src/tokenize.cpp \
           ../src/settings.cpp \
           ../src/errorlogger.cpp \
           ../src/token.cpp \
           ../src/checkautovariables.cpp \
           ../src/checkbufferoverrun.cpp \
           ../src/checkother.cpp \
           ../src/checkclass.cpp \
           ../src/cppcheck.cpp \
           ../src/checkunusedfunctions.cpp \
           ../src/checkdangerousfunctions.cpp \
           ../src/filelister.cpp \
           ../src/mathlib.cpp \
           ../src/checkmemoryleak.cpp \
           ../src/preprocessor.cpp \
           ../src/checkstl.cpp \
           ../src/CheckHeaders.cpp

win32 {
    CONFIG += console
    LIBS += -lshlwapi
}
