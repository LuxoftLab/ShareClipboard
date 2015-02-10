#include "client_connection.h"

QByteArray ClientConnection::makeBinaryPack(pckg_t type, char* dat, int datsize)
{
    char* d;
    d = dat;
    TcpPackageHeader head = TcpPackageHeader(type, datsize);
    TcpPackage pack = TcpPackage(head, d);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

QByteArray ClientConnection::makeBinaryPack(pckg_t type, QString str){
    TcpPackageHeader head = TcpPackageHeader(type, str.size());
    char* dat;
    dat = str.toUtf8().data();
    TcpPackage pack = TcpPackage(head, dat);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

QByteArray ClientConnection::makeBinaryPack(pckg_t type, qint32 num)
{
    TcpPackageHeader head = TcpPackageHeader(type, 4);
    char* dat;
    sprintf(dat, "%d", num);
    TcpPackage pack = TcpPackage(head, dat);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

void ClientConnection::makePass(QDataStream& in)
{
    qint32 pwdsz;
    in >> pwdsz;
    char* pwd = new char[pwdsz];
    in >> pwd;
    qint32 lgnsz;
    in >> lgnsz;
    char* lgn = new char[lgnsz];
    in >> lgn;
    qDebug() << pwd << lgn;
    emit(verifyPass(QString::fromUtf8(pwd, pwdsz), this));
}

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    connect(socket, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
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
    socket->write(makeBinaryPack(REMOVE, addr.toIPv4Address()));
}

QString ClientConnection::getLogin() {
    return login;
}

void ClientConnection::onData(){
    QDataStream in(socket);
    qint32 packt;
    in >> packt;
    if(packt == 0)
        qDebug() << "No data delivered";
    switch(packt){
        case PASS:
            makePass(in);
            break;
        default: throw packt;
    }
}

QHostAddress ClientConnection::makeHostAdress(char* block){
    QHostAddress* address = new QHostAddress;
    return *address;
}
