#-------------------------------------------------
#
# Project created by QtCreator 2011-07-28T16:13:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

HEADERS += \
           core/functions.h \
    gui/basewidgetview.h \
    global/ezcrud_global.h \
    gui/basecrud.h \
    gui/entityview.h \
    core/crudnamespace.h \
    core/modules/plugininterface.h \
    gui/logindialog.h \
    gui/extras/customeditor.h \
    gui/extras/consulta.h\
    core/sql/exception/sqlunknownexception.h \
    core/sql/exception/sqltransactionexception.h \
    core/sql/exception/sqlstatementexception.h \
    core/sql/exception/sqlexception.h \
    core/sql/exception/sqlconnectionexception.h \
    gui/selectserver.h \
    core/session.h \
    gui/mainwindow.h \
    core/modules/basemodule.h \
    gui/navigatorbar.h \
    core/connectionpool.h\
    core/settings.h \
    core/encryption.h \
    core/modules/widgethandler.h \
    gui/mdiwidget.h \
    gui/mdiarea.h \
    core/dynamic/dynamicbinder.h \
    core/dynamic/binder.h \
    core/dynamic/propertybinder.h \
    core/dynamic/widgetbinder.h \
    core/sql/sqlquery.h \
    core/dynamic/viewfactory.h \
    data/objectbase.h \
    data/genericdao.h \
    data/entityclass.h \
    data/dao/widgetdao.h \
    data/dao/revisiondao.h \
    data/dao/moduledao.h \
    data/object/widget.h \
    data/object/revision.h \
    data/object/module.h \
    core/constants.h \
    data/dao/organizationdao.h \
    data/dao/userdao.h \
    data/object/organization.h \
    data/object/user.h \
    core/ezapplication.h \
    data/model/organizationmodel.h \
    gui/editors/organizationeditor.h \
    gui/selectorganization.h \
    data/dao/widget_revisiondao.h \
    data/dao/module_revisiondao.h \
    data/object/widget_revision.h \
    data/object/module_revision.h \
    core/algorithms.h \
    gui/caseutil.h
SOURCES += core/functions.cpp \
    gui/basewidgetview.cpp \
    gui/basecrud.cpp \
    gui/entityview.cpp \
    core/crudnamespace.cpp \
    core/modules/plugininterface.cpp \
    gui/logindialog.cpp \
    gui/extras/customeditor.cpp \
    gui/extras/consulta.cpp\
    core/sql/exception/sqlunknownexception.cpp \
    core/sql/exception/sqltransactionexception.cpp \
    core/sql/exception/sqlstatementexception.cpp \
    core/sql/exception/sqlexception.cpp \
    core/sql/exception/sqlconnectionexception.cpp \
    gui/selectserver.cpp \
    core/session.cpp \
    gui/mainwindow.cpp \
    core/modules/basemodule.cpp \
    gui/navigatorbar.cpp \
    core/connectionpool.cpp\
    core/settings.cpp \
    core/encryption.cpp \
    core/modules/widgethandler.cpp \
    gui/mdiwidget.cpp \
    gui/mdiarea.cpp \
    core/dynamic/dynamicbinder.cpp \
    core/dynamic/binder.cpp \
    core/dynamic/propertybinder.cpp \
    core/dynamic/widgetbinder.cpp \
    core/sql/sqlquery.cpp \
    core/dynamic/viewfactory.cpp \
    data/objectbase.cpp \
    data/entityclass.cpp \
    data/dao/widgetdao.cpp \
    data/dao/revisiondao.cpp \
    data/dao/moduledao.cpp \
    data/object/widget.cpp \
    data/object/revision.cpp \
    data/object/module.cpp \
    data/dao/organizationdao.cpp \
    data/dao/userdao.cpp \
    data/object/organization.cpp \
    data/object/user.cpp \
    core/ezapplication.cpp \
    data/model/organizationmodel.cpp \
    gui/editors/organizationeditor.cpp \
    gui/selectorganization.cpp \
    data/dao/widget_revisiondao.cpp \
    data/dao/module_revisiondao.cpp \
    data/object/widget_revision.cpp \
    data/object/module_revision.cpp \
    gui/caseutil.cpp

FORMS +=  gui/extras/customeditor.ui \
    gui/extras/consulta.ui \
    gui/logindialog.ui \
    gui/basecrud.ui \
    gui/entityview.ui \
    gui/selectserver.ui \
    gui/mainwindow.ui \
    gui/selectorganization.ui

RESOURCES += \
    ezcrud_images.qrc

CONFIG(debug, debug|release) {
    TARGET = ezcrudd
} else {
    TARGET = ezcrud
} # CONFIG(debug, debug|release)

DEFINES += EZCRUD_LIBRARY

###################################
# DIRECTORIO HOME DEL PROYECTO
###################################
HOME_DIR = ?
TARGET_DIR = EZCRUD

## BEGIN_GENERIC
win32 {
HOME_DIR = C:/C++/Dev-Packages
# Create new Directory
#system(rm -rf $${HOME_DIR}/$${TARGET_DIR}/include)
#system(cyg_mkdir -p $${HOME_DIR}/$${TARGET_DIR}/include)
#system(cyg_mkdir -p $${HOME_DIR}/$${TARGET_DIR}/lib)
#winPWD = $$PWD
#winTargetDir = $${HOME_DIR}/$${TARGET_DIR}/
#includePath = $${HOME_DIR}/$${TARGET_DIR}/include
#libPath = $${HOME_DIR}/$${TARGET_DIR}/lib
#winHeaders = $${HEADERS}
#winPWD ~= s,/,\\,g
#winTargetDir ~= s,/,\\,g
#includePath ~= s,/,\\,g
#libPath ~= s,/,\\,g
#winHeaders ~= s,/,\\,g
#system(rmdir /s /q $${winTargetDir})
#system(rmdir /s /q $${includePath})
#system(rmdir /s /q $${libPath})
#system(mkdir $${includePath})
#system(mkdir $${libPath})

#for(header , winHeaders){
#    system(robocopy $${winPWD}\\*.h $${includePath})
#    system(xcopy /y /t $${header} $${includePath})
#    system(xcopy /y /s $${header} $${includePath})
#}
#system(xcopy /y $${winPWD}\\EZCRUD.pri $${winTargetDir})

system(rm -rf $${HOME_DIR}/$${TARGET_DIR}/include)
system(cyg_mkdir -p $${HOME_DIR}/$${TARGET_DIR}/include)
system(cyg_mkdir -p $${HOME_DIR}/$${TARGET_DIR}/lib)
system(cp -rfp --parents $${HEADERS} -t $${HOME_DIR}/$${TARGET_DIR}/include)
system(cp EZCRUD.pri $${HOME_DIR}/$${TARGET_DIR}/)
}
unix {
HOME_DIR = /home/luisvaldes/Dev-Packages
# Create new Directory
system(rm -rf $${HOME_DIR}/$${TARGET_DIR}/include)
system(mkdir -p $${HOME_DIR}/$${TARGET_DIR}/include)
system(mkdir -p $${HOME_DIR}/$${TARGET_DIR}/lib)
system(cp -rfp --parents $${HEADERS} -t $${HOME_DIR}/$${TARGET_DIR}/include)
system(cp EZCRUD.pri $${HOME_DIR}/$${TARGET_DIR}/)
}
# Copy all the headers to the include directory

#system(cp -f $${HOME_DIR}/$${TARGET_DIR}/lib/ezcrudd* -t C:/Industryhacks/C++/Build/Manager++-build/debug)
DESTDIR = $$quote($${HOME_DIR}/$${TARGET_DIR}/lib)

win32 {
CONFIG(debug, debug|release) {
system(cp -f $${HOME_DIR}/$${TARGET_DIR}/lib/ezcrudd.dll -t C:/Industryhacks/C++/Build/Manager++-build/debug)
system(cp -f $${HOME_DIR}/$${TARGET_DIR}/lib/ezcrudd.dll -t D:/Installers/sourceforge/EZCRUD)
#winDir1 = $${HOME_DIR}/$${TARGET_DIR}/lib/ezcrudd.dll
#winDir2 = C:/Industryhacks/C++/Build/Manager++-build/debug
#winDir1 ~= s,/,\\,g
#winDir2 ~= s,/,\\,g
#system(xcopy /y $${winDir1} $${winDir2})
} else {
system(cp -f $${HOME_DIR}/$${TARGET_DIR}/lib/ezcrud.dll -t D:/Installers/sourceforge/EZCRUD)
#winDir1 = $${HOME_DIR}/$${TARGET_DIR}/lib/ezcrudd.dll
#winDir2 = D:/Installers/sourceforge/EZCRUD
#winDir1 ~= s,/,\\,g
#winDir2 ~= s,/,\\,g
#system(xcopy /y $${winDir1} $${winDir2})
} # CONFIG(debug, debug|release)
}


## END_GENERIC

#copia el project include file

win32 {
DEV_PKG_DIR = $$quote(C:/C++/Dev-Packages)
} # win32

OPENSSL_INCLUDE_PATH = $${DEV_PKG_DIR}/OpenSSL/include
OPENSSL_LIB_PATH = $${DEV_PKG_DIR}/OpenSSL/lib

POCO_INCLUDE_PATH = $${DEV_PKG_DIR}/Poco/include
POCO_LIB_PATH = $${DEV_PKG_DIR}/Poco/lib

# Include editors from dev pgk
INCLUDEPATH += $${DEV_PKG_DIR}/EZCRUD/include/gui/editors
INCLUDEPATH += $${DEV_PKG_DIR}/EZCRUD/include

INCLUDEPATH += $${OPENSSL_INCLUDE_PATH}
INCLUDEPATH += $${POCO_INCLUDE_PATH}

LIBS += -L$${OPENSSL_LIB_PATH}
LIBS += -L$${POCO_LIB_PATH}

LIBS += -lcrypto
LIBS += -lssl
LIBS += -lPocoCrypto

QMAKE_CXXFLAGS += -fexceptions
CONFIG(release) {
    QMAKE_CXXFLAGS += -w
} # CONFIG(debug, debug|release)

OTHER_FILES += \
    EZCRUD.txt
