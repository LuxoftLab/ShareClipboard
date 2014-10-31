#include "connection.h"

Connection::Connection(QTcpSocket * socket)
{
    this->socket = socket;
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onData(qint64)));
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

void Connection::onData(qint64){
    QDataStream in(socket);
    TcpPackage pack;
    in >> pack;
    emit(gotData(&pack));
}
