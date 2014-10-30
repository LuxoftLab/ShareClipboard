#include "connection.h"

Connection::Connection(QTcpSocket * socket)
{
    this->socket = socket;
}

QHostAddress Connection::getIpv4()
{
    return socket->peerAddress();
}

QByteArray Connection::makeBinaryPack(pckg_t type, char* dat, int datsize){
    Data d;
    d.rawData = dat;
    TcpPackageHeader head = TcpPackageHeader(type, datsize);
    TcpPackage pack = TcpPackage(head, d);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}
