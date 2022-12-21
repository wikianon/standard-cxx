#-------------------------------------------------
#
# Project created by QtCreator 2011-06-27T19:56:37
#
#-------------------------------------------------

QT       += core gui sql
TEMPLATE = lib


HEADERS += \
    gui/view/personaview.h \
    gui/view/categoriaview.h \
    data/dao/personadao.h \
    data/dao/persona_categoriadao.h \
    data/dao/categoriadao.h \
    data/model/personamodel.h \
    data/model/categoriamodel.h \
    data/object/persona_categoria.h \
    data/object/persona.h \
    data/object/categoria.h \
    gui/editors/personaeditor.h \
    gui/editors/categoriaeditor.h \
    gui/crud/categoriacrud.h \
    modulegeneric.h \
    gui/crud/personacrud.h \
    data/model/categoriaitem.h \
    data/model/categoriacheckmodel.h

SOURCES += \
    gui/view/personaview.cpp \
    gui/view/categoriaview.cpp \
    data/dao/personadao.cpp \
    data/dao/persona_categoriadao.cpp \
    data/dao/categoriadao.cpp \
    data/model/personamodel.cpp \
    data/model/categoriamodel.cpp \
    data/object/persona_categoria.cpp \
    data/object/persona.cpp \
    data/object/categoria.cpp \
    gui/editors/personaeditor.cpp \
    gui/editors/categoriaeditor.cpp \
    gui/crud/categoriacrud.cpp \
    modulegeneric.cpp \
    gui/crud/personacrud.cpp \
    data/model/categoriaitem.cpp \
    data/model/categoriacheckmodel.cpp
FORMS += \
    gui/view/personaview.ui \
    gui/view/categoriaview.ui


TARGET = mod_generic

#CONFIG(debug, debug|release) {
#    TARGET = mod_genericd
#} else {
#    TARGET = mod_genericd
#} # CONFIG(debug, debug|release)

###################################
# DIRECTORIO HOME DEL PROYECTO
###################################
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

INCLUDEPATH += $$quote(C:/Industryhacks/C++/C++/trunk/Manager++/Module-Generico/gui/editors)
