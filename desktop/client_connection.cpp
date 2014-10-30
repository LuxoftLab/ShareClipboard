#include "client_connection.h"

QByteArray ClientConnection::makeBinaryPack(pckg_t type, char* dat, int datsize){
    Data d;
    d.rawData = dat;
    TcpPackageHeader head = TcpPackageHeader(type, datsize);
    TcpPackage pack = TcpPackage(head, d);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

QByteArray ClientConnection::makeBinaryPack(pckg_t type, QString str){
    TcpPackageHeader head = TcpPackageHeader(type, str.size());
    Data dat;
    dat.strData = &str;
    TcpPackage pack = TcpPackage(head, dat);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    connect(socket, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
}

void ClientConnection::sendFail()
{
    socket->write(makeBinaryPack(INVALID_PASS, NULL, 0));
}

void ClientConnection::sendMember(QString login, QHostAddress addr)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << login.size() << login << addr.toString().size() << addr.toString();
    socket->write(makeBinaryPack(MEMBER, dat.data(), dat.size()));
}

void ClientConnection::removeMember(QHostAddress addr)
{
    socket->write(makeBinaryPack(REMOVE, addr.toString()));
}

QString ClientConnection::getLogin() {
    return login;
}

void ClientConnection::emitDeleteMember(){
    emit deleteMember(this->getIpv4());
}

