#include "tcp_server.h"
#include <QNetworkInterface>

TCPServer::TCPServer() : QTcpServer()
{
    //server = new QTcpServer(this);
    if(!this->listen(QHostAddress::Any, PORT_NUMBER))
        qDebug() << "Server is not listening";
    //connect(server, SIGNAL(newConnection()), this, SLOT(newMember()));
}

TCPServer::~TCPServer()
{
    //delete server;
}

void TCPServer::incomingConnection(qintptr socketDescr)
{
    emit addMember(socketDescr);
}

void TCPServer::newMember(){
    //    if((newConnection = server->nextPendingConnection())){
    //        //emit addMember(newConnection);
    //        emit addMember(newConnection->socketDescriptor());
    //    }
}
