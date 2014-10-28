#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    MainWindow w;
    w.show();
    return a.exec();
}

