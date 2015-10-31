#include "tcp_server.h"
#include <QNetworkInterface>

TCPServer::TCPServer() : QTcpServer()
{
    if(!this->listen(QHostAddress::Any, PORT_NUMBER))
        qDebug() << "Server is not listening";
<<<<<<< HEAD
}

TCPServer::~TCPServer()
{}
=======
    connect(server, SIGNAL(newConnection()), this, SLOT(newMember()));

}
/*
QHostAddress TCPServer::getLocalAddress()
{

    return QHostAddress::LocalHost;
}
*/
void TCPServer::newMember(){
    if(newConnection = server->nextPendingConnection()){
        connect(newConnection, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
        emit addMember(newConnection);
    }
}
>>>>>>> origin/interfaces

void TCPServer::incomingConnection(qintptr socketDescr)
{
    emit addMember(socketDescr);
}
