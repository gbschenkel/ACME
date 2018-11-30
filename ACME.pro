QT += core
QT -= gui

TARGET = ACME
CONFIG += c++14 console link_pkgconfig
CONFIG -= app_bundle

PKGCONFIG += libmongocxx

QMAKE_CXXFLAGS += -std=c++14

TEMPLATE = app

SOURCES += main.cpp \
    logreader.cpp \
    config.cpp \
    database.cpp \
    stringhandle.cpp \
    bsonhandler.cpp

HEADERS += \
    logreader.h \
    config.h \
    database.h \
    stringhandle.h \
    definition.h \
    bsonhandler.h \
    mainframe.h \
    payware.h \
    script.h \
    mmd.h

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
