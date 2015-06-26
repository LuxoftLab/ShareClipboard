#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Controller controller(window);
    Controller * controller = new Controller();
    //window->show();

    return a.exec();
}
