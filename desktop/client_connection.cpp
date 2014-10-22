#include "client_connection.h"

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{

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
