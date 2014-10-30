#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    MainWindow window;
    QObject::connect(&window, SIGNAL(roomCreated(QString,QString,QString)),
                     &controller, SLOT(createRoom(QString,QString,QString)));

    QObject::connect(&window, SIGNAL(roomSelected(quint32,QString,QString)),
                     &controller, SLOT(joinRoom(qint32,QString,QString)));
    window.show();
    return a.exec();
}

