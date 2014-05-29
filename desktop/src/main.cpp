#include <QApplication>
#include <QThread>

#include "listener.h"
#include "clientlist.h"
#include "clipboardmanager.h"
#include "logview.h"
#include "userinterface.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

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
    
    listener_thread->start();
    UserInterface ui(peer_list);
    QCoreApplication::connect(&ui,&UserInterface::enableSignal,listener,&Listener::onEnable);
    QCoreApplication::connect(&ui,&UserInterface::enableSignal,peer_list,&ClientList::onEnable);
    QCoreApplication::connect(&ui,&UserInterface::enableSignal,clip_manager,&ClipboardManager::onEnable);

    QCoreApplication::connect(&ui,&UserInterface::disableSignal,listener,&Listener::onDisable);
    QCoreApplication::connect(&ui,&UserInterface::disableSignal,peer_list,&ClientList::onDisable);
    QCoreApplication::connect(&ui,&UserInterface::disableSignal,clip_manager,&ClipboardManager::onDisable);
    return a.exec();
}
