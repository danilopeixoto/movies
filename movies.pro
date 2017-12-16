QT = core widgets gui network

TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = movies

INCLUDEPATH = include
SOURCEPATH = src

HEADERS = \
    $${INCLUDEPATH}/DatabaseFile.h \
    $${INCLUDEPATH}/Global.h \
    $${INCLUDEPATH}/MainWindow.h \
    $${INCLUDEPATH}/MovieCollection.h \
    $${INCLUDEPATH}/MovieData.h \
    $${INCLUDEPATH}/MovieDatabase.h \
    $${INCLUDEPATH}/Movies.h \
    $${INCLUDEPATH}/Runtime.h

SOURCES = \
    $${SOURCEPATH}/DatabaseFile.cpp \
    $${SOURCEPATH}/main.cpp \
    $${SOURCEPATH}/MainWindow.cpp \
    $${SOURCEPATH}/MovieData.cpp \
    $${SOURCEPATH}/MovieDatabase.cpp \
    $${SOURCEPATH}/Runtime.cpp