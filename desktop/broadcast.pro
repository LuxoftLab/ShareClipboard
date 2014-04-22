#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T23:18:35
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = broadcast
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    clientlist.cpp \
    listener.cpp \
    common.cpp \
    logger.cpp

HEADERS += \
    clientlist.h \
    common.h \
    listener.h \
    logger.h
