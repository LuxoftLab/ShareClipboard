#include "clientlist.h"

ClientList::ClientList(QObject *parent) :
    QObject(parent)
{
}

void ClientList::clientFound(QHostAddress host, QString name)
{
    if(m_clients.contains(name))
        return;
    m_clients.insert(name,host);
    Logger::instance()<<TimeStamp()<<"Peer found: "<<host<<" "<<name<<"\n";
}
