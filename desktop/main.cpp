#include <QApplication>

#include "controller.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    //qDebug() << sizeof(QHostAddress);
    return a.exec();
}

