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
    for(QMap<qint32, ClientConnection*>::iterator it = verified.begin(); it != verified.end(); ++it)
        delete it.value();
    for(QMap<qint32, ClientConnection*>::iterator it = notVerified.begin(); it != notVerified.end(); ++it)
        delete it.value();
    delete server;
}

void ServerRoom::addMember(QTcpSocket * socket)
{
    ClientConnection *t = new ClientConnection(socket);
    connect(t, SIGNAL(verifyPass(QString, floating_server_priorities, ClientConnection* const)),
            this, SLOT(verifyPass(QString, floating_server_priorities, ClientConnection* const)));
    connect(t, SIGNAL(onText(QString, ClientConnection * const)),
            this, SLOT(onText(QString, ClientConnection * const)));
    connect(t, SIGNAL(onImage(QByteArray, ClientConnection * const)),
            this, SLOT(onImage(QByteArray, ClientConnection * const)));
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

bool ServerRoom::verifyPass(QString pass, floating_server_priorities priority, ClientConnection * conn)
{
    if(this->pass != pass)
    {
        qDebug() << pass << this->pass;
        conn->sendFail();
        return false;
    }
    qDebug() << "verified IP:" << conn->localAddress();
    qint32 ip = conn->getIpv4().toIPv4Address();
    notVerified.remove(ip);
    verified.insert(ip, conn);
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendMember(conn->getLogin(), priority, conn->getIpv4());
        conn->sendMember(t->getLogin(), priority, t->getIpv4());
    }

    return true;
}

void ServerRoom::onText(QString s, ClientConnection * owner)
{
    saveText();
    sendText(s, owner);
}

void ServerRoom::onImage(QByteArray im, ClientConnection * const)
{
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        //t->sendImage(im);
        t->sendData(im, IMAGE);
    }
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
        //t->sendText(s);
        t->sendData(s.toUtf8(), TEXT);
    }
}
