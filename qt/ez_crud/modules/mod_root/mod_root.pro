TEMPLATE = lib
CONFIG += qt plugin
QT += core
TARGET= mod_root

HEADERS += \
    gui/view/widgetview.h \
    gui/view/moduleview.h \
    gui/editors/moduleeditor.h \
    gui/crud/widgetcrud.h \
    gui/crud/modulecrud.h \
    data/model/accessmodemodel.h \
    data/model/accessmodeitem.h \
    gui/crud/usercrud.h \
    gui/view/userview.h \
    gui/view/organizationview.h \
    gui/crud/organizationcrud.h \
    moduleroot.h

SOURCES += \
    gui/view/widgetview.cpp \
    gui/view/moduleview.cpp \
    gui/editors/moduleeditor.cpp \
    gui/crud/widgetcrud.cpp \
    gui/crud/modulecrud.cpp \
    data/model/accessmodemodel.cpp \
    data/model/accessmodeitem.cpp \
    gui/view/userview.cpp \
    gui/crud/usercrud.cpp \
    gui/view/organizationview.cpp \
    gui/crud/organizationcrud.cpp \
    moduleroot.cpp

FORMS += \
    gui/view/widgetview.ui \
    gui/view/moduleview.ui \
    gui/view/userview.ui \
    gui/view/organizationview.ui

HOME_DIR = /home/luisvaldes/Knut-build

win32 {
HOME_DIR = C:/C++/Dev-Packages/
}
unix {
HOME_DIR = /home/luisvaldes/Knut-build
}

include($${HOME_DIR}/EZCRUD/EZCRUD.pri)

win32 {
CONFIG(debug, debug|release) {
DESTDIR = $$quote(C:/Industryhacks/C++/Build/Manager++-build/debug/modules)
} else {
DESTDIR = $$quote(D:/Installers/sourceforge/EZCRUD/modules)
} # CONFIG(debug, debug|release)
}
unix {
DESTDIR = $$quote(/home/luisvaldes/IndustryHacks/C++/bin/modules)
}

INCLUDEPATH += $$quote($$PWD/)
