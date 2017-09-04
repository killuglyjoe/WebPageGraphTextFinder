#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T22:57:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebPageGraphTextFinder
TEMPLATE = app

#Переводы
CODECFORTR  = UTF-8

TRANSLATIONS = \
    $$PWD/wpgtf_en.ts \
    $$PWD/wpgtf_ru.ts

BUILDDIR = Compiled

DESTDIR += $$PWD/Distr

win32:!wince*: BUILDDIR = Compiled/Win32
win64:!wince*: BUILDDIR = Compiled/Win64
linux-g++:     BUILDDIR = Compiled/Linux

UI_DIR      = $$BUILDDIR/Ui
MOC_DIR     = $$BUILDDIR/MOC
OBJECTS_DIR = $$BUILDDIR/OBJ

LIBDIR = $$PWD/libs

INCLUDEPATH += $$PWD $$LIBDIR
DEPENDPATH += $$PWD $$LIBDIR

include($$LIBDIR/textutils/textutils.pri)
include($$LIBDIR/networkutils/networkutils.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    libs/webpagetextthreadprocessor.cpp

HEADERS  += mainwindow.h \
    libs/webpagetextthreadprocessor.h

FORMS    += mainwindow.ui
