#include "server_connection.h"

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
