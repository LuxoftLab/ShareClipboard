#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket->connectToHost(host, (qint16)getpid()); // I don't know what else to do here
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
              //SLOT(throwSocketError(QAbstractSocket::SocketError)));
    //#todo error handlers
    connect(socket, SIGNAL(readyRead()), this, SLOT(connectToServer()));
}

void ServerConnection::sendPassAndLogin(QString password, QString login)
{
    //#how to ensure socket is ready to write?
    //if(socket->ConnectedState)??
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    Data dat;
    dat.pass = &password;
    TcpPackage loginPckg = TcpPackage(TcpPackageHeader(LOGIN, password.size()), dat);
    out << loginPckg;
    socket->write(block);

    dat.strData = &login;
    TcpPackage passwordPckg = TcpPackage(TcpPackageHeader(PASS, login.size()), dat);
    out << passwordPckg;
    socket->write(block);

    //#socket->disconnectFromHost(); should I?
}
