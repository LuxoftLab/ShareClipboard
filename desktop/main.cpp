#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    //controller.createRoom("test", "login", "pass");
    return a.exec();
}

