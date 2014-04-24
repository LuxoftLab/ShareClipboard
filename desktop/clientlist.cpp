#include "clientlist.h"

ClientList::ClientList(QObject *parent) :
    QObject(parent)
{
    m_timer.setInterval(PEER_TIMER_TIMEOUT);
    connect(&m_timer,&QTimer::timeout,this,&ClientList::onTimerTriggered);
    m_timer.start();
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
    QElapsedTimer * elapsed=new QElapsedTimer();
    elapsed->start();
    m_elapsed_timers.insert(name,elapsed);
    Logger::instance()<<TimeStamp()<<"Peer found: "<<host<<" "<<name<<"\n";
    updatePeerMenu();
}

void ClientList::helloReceived(QHostAddress host, QString name)
{
    QMutexLocker lock(&m_guard);
    if(m_elapsed_timers.contains(name))
        m_elapsed_timers[name]->restart();
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
    foreach (QString key, m_elapsed_timers.keys()) {
        QElapsedTimer * timer=m_elapsed_timers[key];
        if(timer->hasExpired(PEER_OFFLINE_TIMEOUT))
        {
            timer->invalidate();
            delete timer;
            m_elapsed_timers.remove(key);
            m_clients.remove(key);
        }
        if(timer->hasExpired(PEER_CHECK_TIMEOUT))
            emit sendAreYouHere(m_clients[key]);
    }
    updatePeerMenu();
}

void ClientList::onEnable()
{

}

void ClientList::onDisable()
{

}

void ClientList::updatePeerMenu()
{
    m_peers_list.clear();
    foreach (QString name, m_clients.keys()) {
        m_peers_list.addAction(QString("%1 (%2)").arg(name,m_clients[name].toString()));
    }
}
