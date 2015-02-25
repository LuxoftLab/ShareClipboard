#include "tcp_server.h"
#include <QNetworkInterface>

TCPServer::TCPServer()
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, PORT_NUMBER))
        qDebug() << "Server is not listening";
    connect(server, SIGNAL(newConnection()), this, SLOT(newMember()));

}

TCPServer::~TCPServer()
{
    delete newConnection;
    delete server;
}

QHostAddress TCPServer::getLocalAddress()
{
    return QHostAddress::LocalHost;
}

void TCPServer::newMember(){
    if(newConnection = server->nextPendingConnection()){
        connect(newConnection, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
        emit addMember(newConnection);
    }
}
