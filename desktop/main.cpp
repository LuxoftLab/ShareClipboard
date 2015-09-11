#include <QApplication>
#include <QObject>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClipboardTrayIcon * icon = new ClipboardTrayIcon();
    Controller controller(icon);

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
