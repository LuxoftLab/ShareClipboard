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
    connection->sendPassAndLogin(pass, login);
}

void ClientRoom::addMember(QString login, QHostAddress addr) {
    members.insert(addr.toIPv4Address(), new Member(login, addr));
}

void ClientRoom::deleteMember(QHostAddress addr) {
    members.remove(addr.toIPv4Address());
}

ClientRoom::~ClientRoom() {
    delete connection;
}

Member::Member(QString login, QHostAddress addr) {
    this->login = login;
    this->addr = addr;
}

