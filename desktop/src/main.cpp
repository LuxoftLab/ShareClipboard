#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QThread>

#include "listener.h"
#include "clientlist.h"
#include "clipboardmanager.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    QApplication a(argc, argv);

    QThread * listener_thread=new QThread();
    Listener * listener=new Listener();
    ClientList * peer_list=new ClientList();
    ClipboardManager * clip_manager=new ClipboardManager();
    listener->moveToThread(listener_thread);
    QCoreApplication::connect(listener_thread,&QThread::started,listener,&Listener::listen);
    QCoreApplication::connect(listener_thread,&QThread::finished,listener,&Listener::deleteLater);
    QCoreApplication::connect(listener_thread,&QThread::finished,listener_thread,&QThread::deleteLater);

    QCoreApplication::connect(listener,&Listener::peerFound,peer_list,&ClientList::clientFound);
    QCoreApplication::connect(listener,&Listener::helloReceived,peer_list,&ClientList::clientFound);
    QCoreApplication::connect(listener,&Listener::clipboardContentArrived,clip_manager,&ClipboardManager::clipboardContentArrived);
    QCoreApplication::connect(clip_manager,&ClipboardManager::sendClipboard,peer_list,&ClientList::onSendClipboard);
    QCoreApplication::connect(peer_list,&ClientList::sendClipboard,listener,&Listener::sendClipboard);
    QCoreApplication::connect(peer_list,&ClientList::sendAreYouHere,listener,&Listener::sendAreYouHere);
    QCoreApplication::connect(listener,&Listener::resetTimerForPeer,peer_list,&ClientList::resetTimerForPeer);

    QSystemTrayIcon tray_icon(QIcon(":/img/clipboard-big.png"));
    tray_icon.setContextMenu(peer_list->contextMenu());
    tray_icon.show();

    listener_thread->start();
    return a.exec();
}
