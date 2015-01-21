#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    RoomsListDialog rooms;

    //create room implementation
    QObject::connect(&rooms, SIGNAL(newRoomCreated(QString,QString)), &controller, SLOT(createServerRoom(QString,QString)));
    //join room implementation
    QObject::connect(&controller, SIGNAL(roomAdded(QString,qint32)), &rooms, SLOT(addRoom(QString,qint32)));

    //delete room implementation
    QObject::connect(&controller, SIGNAL(serverIsUp(QString)), &rooms, SLOT(onServerIsUp(QString)));
    QObject::connect(&rooms, SIGNAL(deleteServerRoom()), &controller, SLOT(deleteServerRoom()));
    QObject::connect(&controller, SIGNAL(roomDeleted(QString)), &rooms, SLOT(deleteRoom(QString)));

    rooms.show();
    return a.exec();
}

