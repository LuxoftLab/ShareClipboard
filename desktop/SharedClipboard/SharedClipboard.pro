TEMPLATE = app

QT += qml quick widgets network

SOURCES += main.cpp \
    udpservice.cpp \
    encoder.cpp \
    tcpservice.cpp \
    control.cpp \
    clipboardservice.cpp \
    fileservice.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    udpservice.h \
    encoder.h \
    tcpservice.h \
    tcppackages.h \
    control.h \
    shared_clipboard_exception.h \
    clipboardservice.h \
    fileservice.h \
    common.h

CONFIG += c++11
