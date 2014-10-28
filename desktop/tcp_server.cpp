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
    if(server)
        return server->serverAddress();
    else
       qDebug() << "server is not initialized";
}

void TCPServer::newMember(){
    if(QTcpSocket* newConnection = server->nextPendingConnection())
        emit addMember(newConnection);
}


