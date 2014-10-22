#include "connection.h"

Connection::Connection(QUdpSocket * socket)
{
    this->socket = socket;
}

qint32 Connection::getIpc4()
{
    return socket->peerAddress().toIPv4Address();
}
