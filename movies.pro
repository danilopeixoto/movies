QT = core widgets qml quick network singleapplication
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = movies
VERSION = 1.0.0

QMAKE_TARGET_PRODUCT = Movies
QMAKE_TARGET_COMPANY = "Danilo Ferreira"
QMAKE_TARGET_DESCRIPTION = Movies
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2018, Danilo Ferreira. All rights reserved."

PROJECTPATH = $$PWD
INCLUDEPATH = include
SOURCEPATH = src
RESOURCEPATH = res
BUILDPATH = $$OUT_PWD

HEADERS = \
    $${INCLUDEPATH}/File.h \
    $${INCLUDEPATH}/Global.h \
    $${INCLUDEPATH}/MovieData.h \
    $${INCLUDEPATH}/MovieDatabase.h \
    $${INCLUDEPATH}/MovieLibrary.h \
    $${INCLUDEPATH}/Movies.h \
    $${INCLUDEPATH}/Runtime.h

SOURCES = \
    $${SOURCEPATH}/File.cpp \
    $${SOURCEPATH}/main.cpp \
    $${SOURCEPATH}/MovieData.cpp \
    $${SOURCEPATH}/MovieDatabase.cpp \
    $${SOURCEPATH}/MovieLibrary.cpp \
    $${SOURCEPATH}/Runtime.cpp

RESOURCES = $${RESOURCEPATH}/resources.qrc

RC_LANG = en_US.UTF-8
RC_ICONS = $${RESOURCEPATH}/icons/movies.ico

OBJECTS_DIR = $$BUILDPATH
RCC_DIR = $$BUILDPATH
MOC_DIR = $$BUILDPATH

DEPLOYTOOL = windeployqt
INSTALLTOOL = makensis

DEPLOYPATH = $${BUILDPATH}/release
QMLDIR = $${PROJECTPATH}/$${RESOURCEPATH}/ui

INSTALLSCRIPT = $${PROJECTPATH}/$${RESOURCEPATH}/scripts/setup.nsi

CONFIG(release, debug | release) {
    QMAKE_POST_LINK = $${DEPLOYTOOL} --release --qmldir $${QMLDIR} $${DEPLOYPATH} &
    QMAKE_POST_LINK += $${INSTALLTOOL} $${INSTALLSCRIPT}
}