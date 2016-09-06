QT += core sql
QT -= gui

TARGET = ACME
CONFIG += console c++11
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

SOURCES += main.cpp \
    logreader.cpp \
    config.cpp \
    database.cpp \
    stringhandle.cpp \
    json.cpp

HEADERS += \
    logreader.h \
    config.h \
    database.h \
    stringhandle.h \
    json.h \
    definition.h

DISTFILES +=

