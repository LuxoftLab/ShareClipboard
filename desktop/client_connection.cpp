#include "client_connection.h"

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{

}

void ClientConnection::sendFail()
{

}

void ClientConnection::sendMember(QString login, QHostAddress addr)
{

}

void ClientConnection::removeMember(QHostAddress addr)
{

}

QString ClientConnection::getLogin() {
    return login;
}
