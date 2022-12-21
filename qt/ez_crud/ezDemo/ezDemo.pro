#-------------------------------------------------
#
# Project created by QtCreator 2012-10-25T21:53:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ezDemo
TEMPLATE = app


SOURCES += main.cpp

win32 {
DEV_PKG_DIR = $$quote(C:/C++/Dev-Packages)
RC_FILE = ezdemo.rc
} # win32

win32 {
CONFIG(debug, debug|release) {
} else {
DESTDIR = $$quote(D:/Installers/sourceforge/EZCRUD)
} # CONFIG(debug, debug|release)
}
unix {
DESTDIR = $$quote(/home/luisvaldes/IndustryHacks/C++/bin/modules)
}

include($${DEV_PKG_DIR}/EZCRUD/EZCRUD.pri)
