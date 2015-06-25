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
    delete server;
}

void TCPServer::newMember(){
    if((newConnection = server->nextPendingConnection())){
        emit addMember(newConnection);
    }
}
