#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    RoomsListDialog rooms;
    QObject::connect(&rooms, SIGNAL(newRoomCreated(QString,QString)), &controller, SLOT(createServerRoom(QString,QString)));
    QObject::connect(&rooms, SIGNAL(roomChoosed(qint32,QString)), &controller, SLOT(joinRoom(qint32,QString)));
    QObject::connect(&rooms, SIGNAL(deleteRoomNameGet(QString)), &controller, SLOT(deleteRoom(QString)));
    QObject::connect(&controller, SIGNAL(roomAdded(QString,qint32)), &rooms, SLOT(addRoom(QString,qint32)));
    QObject::connect(&controller, SIGNAL(roomDeleted(QString)), &rooms, SLOT(deleteRoom(QString)));
    QObject::connect(&controller, SIGNAL(serverIsUp(QString)), &rooms, SLOT(buttonView(QListWidgetItem*)));
    rooms.show();
    return a.exec();
}

