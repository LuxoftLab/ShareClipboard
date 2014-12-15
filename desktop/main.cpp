#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    RoomsListDialog rooms;
    QObject::connect(&rooms, SIGNAL(newRoomCreated(QString,QString)), &controller, SLOT(createRoom(QString,QString)));
    QObject::connect(&rooms, SIGNAL(roomChoosed(qint32,QString)), &controller, SLOT(joinRoom(qint32,QString)));
    QObject::connect(&controller, SIGNAL(roomAdded(QString,qint32)), &rooms, SLOT(addRoom(QString,qint32)));
    rooms.show();
    return a.exec();
}

