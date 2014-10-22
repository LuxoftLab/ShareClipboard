#include "client_connection.h"

ClientConnection::ClientConnection(ServerRoom * room, QUdpSocket * socket) : Connection(socket)
{
    this->room = room;
}

void ClientConnection::sendFail()
{

}

void ClientConnection::sendMember(QString login, quint32 ipv4)
{

}

QString ClientConnection::getLogin() {
    return login;
}
