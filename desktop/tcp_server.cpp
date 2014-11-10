#include "tcp_server.h"

TCPServer::TCPServer()
{
    server = new QTcpServer(this);
    if(!server->listen())
        qDebug() << "Server is not listening";
    connect(server, SIGNAL(newConnection()), this, SLOT(newMember()));

}

QHostAddress TCPServer::getLocalAddress()
{

    return QHostAddress::LocalHost;
}

void TCPServer::newMember(){
    if(QTcpSocket* newConnection = server->nextPendingConnection()){
        connect(newConnection, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
        emit addMember(newConnection);
    }
}

void TCPServer::emitDeleteMember(){

}

