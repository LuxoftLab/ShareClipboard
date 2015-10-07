#include "tcp_server.h"
#include <QNetworkInterface>

TCPServer::TCPServer() : QTcpServer()
{
    if(!this->listen(QHostAddress::Any, PORT_NUMBER))
        qDebug() << "Server is not listening";
}

TCPServer::~TCPServer()
{}

void TCPServer::incomingConnection(qintptr socketDescr)
{
    emit addMember(socketDescr);
}
