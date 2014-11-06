#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket->connectToHost(host, PORT_NUMBER);
    //connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
              //SLOT(throwSocketError(QAbstractSocket::SocketError)));
    //#todo error handlers
    //connect(socket, SIGNAL(readyRead()), this, SLOT(connectToServer()));
    //  connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onData(qint64)));
}


void ServerConnection::sendPassAndLogin(QString password, QString login){
   QByteArray dat;
   QDataStream out(&dat, QIODevice::WriteOnly);
   out << login.toUtf8().size() << login.toUtf8() << password.toUtf8().size() << password.toUtf8();
   socket->write(makeBinaryPack(PASS, dat.data(), dat.size()));
}

void ServerConnection::deleteMe(QHostAddress address){
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << address.toIPv4Address();
    socket->write(makeBinaryPack(REMOVE, dat.data(), dat.size()))   ;
}
