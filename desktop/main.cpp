#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QThread>

#include "listener.h"
#include "clientlist.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    QApplication a(argc, argv);

    QThread * listener_thread=new QThread();
    Listener * listener=new Listener();
    ClientList * peer_list=new ClientList();
    listener->moveToThread(listener_thread);
    QCoreApplication::connect(listener_thread,&QThread::started,listener,&Listener::listen);
    QCoreApplication::connect(listener_thread,&QThread::finished,listener,&Listener::deleteLater);
    QCoreApplication::connect(listener_thread,&QThread::finished,listener_thread,&QThread::deleteLater);
    QCoreApplication::connect(listener,&Listener::peerFound,peer_list,&ClientList::clientFound);
    QCoreApplication::connect(listener,&Listener::helloReceived,peer_list,&ClientList::clientFound);
    QCoreApplication::connect(peer_list,&ClientList::sendHello,listener,&Listener::sendHello);

    QSystemTrayIcon tray_icon(QIcon(":/img/clipboard.png"));
    tray_icon.setContextMenu(peer_list->contextMenu());
    tray_icon.show();

    listener_thread->start();
    return a.exec();
}
