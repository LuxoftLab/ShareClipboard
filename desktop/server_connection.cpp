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
    /*QByteArray block;
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
    */

}
