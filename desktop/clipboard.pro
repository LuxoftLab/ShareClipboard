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
    client_connection.cpp \
    client_room.cpp \
    connection.cpp \
    controller.cpp \
    room.cpp \
    server_connection.cpp \
    server_room.cpp \
    tcp_server.cpp \
    udp_packet.cpp \
    udp_service.cpp \
    gui/mainwindow.cpp \
    gui/passworddialog.cpp \
    gui/roomslistdialog.cpp \
    gui/createroomdialog.cpp \
    gui/changenamedialog.cpp

HEADERS  += \
    client_connection.h \
    client_room.h \
    connection.h \
    constants.h \
    controller.h \
    fails.h \
    room.h \
    server_connection.h \
    server_room.h \
    tcp_server.h \
    udp_packet.h \
    udp_service.h \
    gui/mainwindow.h \
    gui/passworddialog.h \
    gui/roomslistdialog.h \
    gui/createroomdialog.h \
    gui/changenamedialog.h \


RESOURCES += \
    gui/images.qrc

FORMS += \
    gui/mainwindow.ui \
    gui/passworddialog.ui \
    gui/roomslistdialog.ui \
    gui/createroomdialog.ui \
    gui/changenamedialog.ui
