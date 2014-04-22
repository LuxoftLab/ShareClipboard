#include "clientlist.h"

ClientList::ClientList(QObject *parent) :
    QObject(parent)
{
    m_timer.setInterval(PEER_LIST_TIMEOUT);
    connect(&m_timer,&QTimer::timeout,this,&ClientList::onTimerTriggered);
    m_peers_list.setTitle("Peer list");
    m_menu.addMenu(&m_peers_list);
}

QMenu *ClientList::contextMenu()
{
    return &m_menu;
}

void ClientList::clientFound(QHostAddress host, QString name)
{
    if(m_clients.contains(name))
    {
        helloReceived(host,name);
        return;
    }
    QMutexLocker lock(&m_guard);
    m_clients.insert(name,host);
    m_timer.start();
    Logger::instance()<<TimeStamp()<<"Peer found: "<<host<<" "<<name<<"\n";
    updatePeerMenu();
}

void ClientList::helloReceived(QHostAddress host, QString name)
{
    QMutexLocker lock(&m_guard);
    m_marked_for_delete.removeAll(name);
}

void ClientList::onSendClipboard(QString text)
{
    QMutexLocker lock(&m_guard);
    foreach (QHostAddress addr, m_clients.values()) {
        emit sendClipboard(addr,text);
    }
}

void ClientList::onTimerTriggered()
{
    QMutexLocker lock(&m_guard);
    while(!m_marked_for_delete.isEmpty())
    {
        m_clients.remove(m_marked_for_delete.first());
        m_marked_for_delete.pop_front();
    }
    foreach (QString name, m_clients.keys()) {
        m_marked_for_delete.push_back(name);
        emit sendAreYouHere(m_clients[name]);
    }
    updatePeerMenu();
}

void ClientList::updatePeerMenu()
{
    m_peers_list.clear();
    foreach (QString name, m_clients.keys()) {
        m_peers_list.addAction(QString("%1 (%2)").arg(name,m_clients[name].toString()));
    }
}
