#include "tcp_server.h"

TCPServer::TCPServer()
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, PORT_NUMBER))
        qDebug() << "Server is not listening";
    connect(server, SIGNAL(newConnection()), this, SLOT(newMember()));

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

void TCPServer::emitDeleteMember(){
    deleteMember(newConnection->peerAddress());
}

