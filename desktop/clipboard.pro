#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T18:07:06
#
#-------------------------------------------------

QT       += core gui network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = clipboard
TEMPLATE = app


SOURCES += main.cpp \
    controller.cpp \
    udp_service.cpp \
    room.cpp \
    server_room.cpp \
    tcp_server.cpp \
    client_room.cpp \
    connection.cpp \
    server_connection.cpp \
    client_connection.cpp \
    tcp_package.cpp

HEADERS  += \
    controller.h \
    udp_service.h \
    room.h \
    server_room.h \
    tcp_server.h \
    client_room.h \
    connection.h \
    server_connection.h \
    client_connection.h \
    udp_packet.h \
    fails.h \
    tcp_package.h
