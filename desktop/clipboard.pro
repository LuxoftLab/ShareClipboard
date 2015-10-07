#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T18:07:06
#
#-------------------------------------------------

QT       += core gui network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = clipboard
TEMPLATE = app
CONFIG += c++11 debug

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
    gui/changenamedialog.cpp \
    clipboard_service.cpp \
    gui/settingsdialog.cpp \
    gui/clipboardtrayicon.cpp \
    gui/about.cpp \
    sharedPackages/package.cpp \
    sharedPackages/memberpackage.cpp \
    sharedPackages/failpackage.cpp \
    sharedPackages/datapackage.cpp \
    sharedPackages/removemember.cpp \
    sharedPackages/file_notification_package.cpp \
    sharedPackages/file_request_package.cpp \
    sharedPackages/file_response_package.cpp \
    sharedPackages/password_package.cpp \
    sharedPackages/image_package.cpp \
    sharedPackages/package_factory.cpp

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
    clipboard_service.h \
    gui/settingsdialog.h \
    gui/clipboardtrayicon.h \
    gui/about.h \
    sharedPackages/package.h \
    sharedPackages/memberpackage.h \
    sharedPackages/failpackage.h \
    sharedPackages/datapackage.h \
    sharedPackages/removemember.h \
    sharedPackages/file_notification_package.h \
    sharedPackages/file_request_package.h \
    sharedPackages/file_response_package.h \
    sharedPackages/password_package.h \
    sharedPackages/image_package.h \
    sharedPackages/package_factory.h

RESOURCES += \
    gui/images.qrc

FORMS += \
    gui/mainwindow.ui \
    gui/passworddialog.ui \
    gui/roomslistdialog.ui \
    gui/createroomdialog.ui \
    gui/changenamedialog.ui \
    gui/settingsdialog.ui \
    about.ui \
    gui/about.ui
