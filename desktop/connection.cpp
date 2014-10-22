#include "connection.h"

Connection::Connection(QTcpSocket * socket)
{
    this->socket = socket;
}

qint32 Connection::getIpc4()
{
    return socket->peerAddress().toIPv4Address();
}
