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
    if(m_clients.contains(host))
        return;
    QMutexLocker lock(&m_guard);
    m_clients.insert(host,name);
    QElapsedTimer * elapsed=new QElapsedTimer();
    elapsed->start();
    m_elapsed_timers.insert(host,elapsed);
    Logger::instance()<<TimeStamp()<<"Peer found: "<<host<<" "<<name<<Flush();
    updatePeerMenu();
}

void ClientList::resetTimerForPeer(QHostAddress peer)
{
    QMutexLocker lock(&m_guard);
    if(m_elapsed_timers.contains(peer))
    {
        Logger::instance()<<TimeStamp()<<"Reset timer for "<<peer.toString()<<Flush();
        m_elapsed_timers[peer]->restart();
        m_possible_timeout.removeAll(peer);
    }
}

void ClientList::onSendClipboard(QString text)
{
    QMutexLocker lock(&m_guard);
    foreach (QHostAddress addr, m_clients.keys()) {
        emit sendClipboard(addr,text);
    }
}

void ClientList::onTimerTriggered()
{
    QMutexLocker lock(&m_guard);
    foreach (QHostAddress key, m_elapsed_timers.keys()) {
        QElapsedTimer * timer=m_elapsed_timers[key];
        if(timer->hasExpired(PEER_CHECK_TIMEOUT))
        {
            if(m_possible_timeout.contains(key))
            {
                timer->invalidate();
                delete timer;
                m_elapsed_timers.remove(key);
                m_clients.remove(key);
                m_possible_timeout.removeAll(key);
                Logger::instance()<<TimeStamp()<<QString("Peer timeout: %1").arg(key.toString())<<Flush();
                continue;
            }
            Logger::instance()<<TimeStamp()<<"Are you here? for "<<key.toString()<<Flush();
            m_possible_timeout.push_back(key);
            timer->restart();
            emit sendAreYouHere(key);
        }
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
    foreach (QHostAddress key, m_clients.keys()) {
        m_peers_list.addAction(QString("%1 (%2)").arg(key.toString(),m_clients[key]));
    }
}
