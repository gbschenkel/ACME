QT += core sql
QT -= gui

TARGET = Druid
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    logreader.cpp \
    config.cpp \
    database.cpp \
    stringhandle.cpp

HEADERS += \
    logreader.h \
    config.h \
    database.h \
    stringhandle.h

DISTFILES +=

