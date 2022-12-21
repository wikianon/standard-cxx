DEV_PKG_DIR = ""
win32 {
DEV_PKG_DIR = $$quote(C:/C++/Dev-Packages)
}

unix {
DEV_PKG_DIR = /home/luisvaldes/Dev-Packages
}

###############################
# OpenSSL Library Configuration #
###############################

OPENSSL_INCLUDE_PATH = $$quote($${DEV_PKG_DIR}/OpenSSL/include)
OPENSSL_LIB_PATH = $$quote($${DEV_PKG_DIR}/OpenSSL/lib)

###############################
# PoCo C++ Library Configuration #
###############################

POCO_INCLUDE_PATH = $$quote($${DEV_PKG_DIR}/Poco/include)
POCO_LIB_PATH = $$quote($${DEV_PKG_DIR}/Poco/lib)

###############################
# knut Library Configuration #
###############################

EZCRUD_INCLUDE_PATH = $$quote($${DEV_PKG_DIR}/EZCRUD/include)
EZCRUD_LIB_PATH = $$quote($${DEV_PKG_DIR}/EZCRUD/lib)
EZCRUD_LIB_DEBUG = "ezcrudd"
EZCRUD_LIB_RELEASE = "ezcrud"

######################
# Globals Parameters #
######################
DEFINES += EZCRUD_LIBRARY

QT += network xml sql gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#############################
# Compiler / Linker Options #
#############################

win32 {
CONFIG(debug, debug|release) {
} else {
DEFINES += NDEBUG
win32-msvc2005: QMAKE_LFLAGS += /OPT:NOREF
win32-msvc2008: QMAKE_LFLAGS += /OPT:NOREF
win32-msvc2010: QMAKE_LFLAGS += /OPT:NOREF
} # CONFIG(debug, debug|release)
win32-g++: QMAKE_LFLAGS += -export-all-symbols -Wl,-enable-auto-import
} # win32

#######################
# Externals Libraries #
#######################
INCLUDEPATH += $${OPENSSL_INCLUDE_PATH}
INCLUDEPATH += $${POCO_INCLUDE_PATH}
INCLUDEPATH += $${EZCRUD_INCLUDE_PATH}

LIBS += -L$${OPENSSL_LIB_PATH}
LIBS += -L$${POCO_LIB_PATH}
LIBS += -L$${EZCRUD_LIB_PATH}

LIBS += -lcrypto
LIBS += -lssl
LIBS += -lPocoCrypto

CONFIG(debug, debug|release) {
LIBS += -l$${EZCRUD_LIB_DEBUG}
} else {
LIBS += -l$${EZCRUD_LIB_RELEASE}
} # CONFIG(debug, debug|release)
