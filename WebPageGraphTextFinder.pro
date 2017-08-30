#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T22:57:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebPageGraphTextFinder
TEMPLATE = app

BUILDDIR = Compiled

DESTDIR += $$PWD/Distr

win32:!wince*: BUILDDIR = Compiled/Win32
win64:!wince*: BUILDDIR = Compiled/Win64
linux-g++:     BUILDDIR = Compiled/Linux

UI_DIR      = $$BUILDDIR/Ui
MOC_DIR     = $$BUILDDIR/MOC
OBJECTS_DIR = $$BUILDDIR/OBJ

LIBDIR = $$PWD/libs

include($$LIBDIR/textutils/textutils.pri)
include($$LIBDIR/networkutils/networkutils.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
