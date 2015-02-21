#include "server_room.h"

ServerRoom::ServerRoom(QString name, QString pass) : Room(name, pass)
{
    server = new TCPServer();
    connect(server, SIGNAL(addMember(QTcpSocket*)),
            this, SLOT(addMember(QTcpSocket*)));
    connect(server, SIGNAL(deleteMember(QHostAddress)),
            this, SLOT(deleteMember(QHostAddress)));
}

ServerRoom::~ServerRoom()
{

}

void ServerRoom::addMember(QTcpSocket * socket)
{
    ClientConnection *t = new ClientConnection(socket);
    connect(t, SIGNAL(verifyPass(QString, ClientConnection* const)),
            this, SLOT(verifyPass(QString, ClientConnection* const)));
    connect(t, SIGNAL(onText(QString, ClientConnection * const)),
            this, SLOT(onText(QString, ClientConnection * const)));
    notVerified.insert(socket->peerAddress().toIPv4Address(), t);
}

void ServerRoom::deleteMember(QHostAddress addr)
{
    qint32 ip = addr.toIPv4Address();
    ClientConnection *t = notVerified.value(ip, NULL);
    if(t != NULL)
    {
        notVerified.remove(ip);
        return;
    }
    t = verified.value(ip, NULL);
    if(t != NULL)
    {
        verified.remove(ip);
        for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
        {
            ClientConnection* c = it.value();
            c->removeMember(addr);
        }
        return;
    }
}

bool ServerRoom::verifyPass(QString pass, ClientConnection * conn)
{
    if(this->pass != pass)
    {
        conn->sendFail();
        return false;
    }
    qint32 ip = conn->getIpv4().toIPv4Address();
    notVerified.remove(ip);
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendMember(conn->getLogin(), conn->getIpv4());
        conn->sendMember(t->getLogin(), t->getIpv4());
    }
    verified.insert(ip, conn);
    return true;
}

void ServerRoom::onText(QString s, ClientConnection * owner)
{
    saveText();
    sendText(s, owner);
}

void ServerRoom::saveText()
{

}

void ServerRoom::sendText(QString s, ClientConnection * owner)
{
    //TODO exclude owner
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendText(s);
    }
}

QHostAddress ServerRoom::getAddr()
{
    return server->getLocalAddress();
}

