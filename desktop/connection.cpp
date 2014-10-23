#include "connection.h"

Connection::Connection(QTcpSocket * socket)
{
    this->socket = socket;
}

QHostAddress Connection::getIpv4()
{
    return socket->peerAddress();
}
