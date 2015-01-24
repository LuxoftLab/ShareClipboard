#include "server_connection.h"

QByteArray ServerConnection::makeBinaryPack(pckg_t type, char* dat, int datsize){
    Data d;
    d.rawData = dat;
    TcpPackageHeader head = TcpPackageHeader(type, datsize);
    TcpPackage pack = TcpPackage(head, d);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket->connectToHost(host, PORT_NUMBER);
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
              //SLOT(throwSocketError(QAbstractSocket::SocketError)));
    //#todo error handlers
    connect(socket, SIGNAL(readyRead()), this, SLOT(connectToServer()));
}

void ServerConnection::sendPassAndLogin(QString password, QString login)
{
   QByteArray dat;
   QDataStream out(&dat, QIODevice::WriteOnly);
   out << login.size() << login << password.size() << password;
   socket->write(makeBinaryPack(PASS, dat.data(), dat.size()));
}

void ServerConnection::sendText(QString text)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << text.size() << text;
    socket->write(makeBinaryPack(TEXT, dat.data(), dat.size()));
}
