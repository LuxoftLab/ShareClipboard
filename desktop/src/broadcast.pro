#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T23:18:35
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = clipboard
#CONFIG   += console
#CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    clientlist.cpp \
    listener.cpp \
    common.cpp \
    logger.cpp \
    clipboardmanager.cpp \
    userinterface.cpp \
    launcher.cpp

HEADERS += \
    clientlist.h \
    common.h \
    listener.h \
    logger.h \
    clipboardmanager.h \
    userinterface.h \
    launcher.h

RESOURCES += \
    rsrc.qrc
