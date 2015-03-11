#include "client_room.h"

ClientRoom::ClientRoom(QString name, QHostAddress host) : Room(name, "")
{
    this->host = host;
}

void ClientRoom::connectToHost(QString login, QString pass)
{
    this->pass = pass;
    connection = new ServerConnection(host);
    connect(connection, SIGNAL(addMember(QString, QHostAddress)),
            this, SLOT(addMember(QString, QHostAddress)));
    connect(connection, SIGNAL(deleteMember(QHostAddress)),
            this, SLOT(deleteMember(QHostAddress)));
    connect(connection->hand, SIGNAL(gotText(QByteArray,QString)),
            this, SIGNAL(gotText(QByteArray, QString)));

    connection->sendPassAndLogin(pass, login);
}

void ClientRoom::addMember(QString login, QHostAddress addr) {
    members.insert(addr.toIPv4Address(), new Member(login, addr));
}

void ClientRoom::deleteMember(QHostAddress addr) {
    members.remove(addr.toIPv4Address());
}

void ClientRoom::sendText(QString text)
{
    connection->sendText(text);
}

void ClientRoom::sendImage(QByteArray)
{
//    /connection->send
}

ClientRoom::~ClientRoom()
{
    for(QMap<qint32, Member*>::iterator it = members.begin(); it != members.end(); ++it)
        delete it.value();
    delete connection;
}

Member::Member(QString login, QHostAddress addr)
{
    this->login = login;
    this->addr = addr;
}

