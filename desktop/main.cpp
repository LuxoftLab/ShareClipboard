#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow * window = new MainWindow();
    Controller controller(window);

    //RoomsListDialog roomsDialog;


//    QObject::connect(&roomsDialog, SIGNAL(newRoomCreated(QString,QString)), &controller, SLOT(createServerRoom(QString,QString)));
//    QObject::connect(&controller, SIGNAL(roomAdded(QString,qint32)), &roomsDialog, SLOT(addRoom(QString,qint32)));
//    QObject::connect(&controller, SIGNAL(serverIsUp(QString)), &roomsDialog, SLOT(onServerIsUp(QString)));
//    QObject::connect(&roomsDialog, SIGNAL(deleteServerRoom()), &controller, SLOT(deleteServerRoom()));
//    QObject::connect(&controller, SIGNAL(roomDeleted(QString)), &roomsDialog, SLOT(deleteRoom(QString)));
//    QObject::connect(&roomsDialog, SIGNAL(roomChoosed(qint32, QString)), &controller, SLOT(joinRoom(qint32,QString)));

    //roomsDialog.show();
    window->show();

    return a.exec();
}

