#include "tcp_server.h"
#include <QNetworkInterface>

TCPServer::TCPServer()
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, PORT_NUMBER))
        qDebug() << "Server is not listening";
    connect(server, SIGNAL(newConnection()), this, SLOT(newMember()));

}

QHostAddress TCPServer::getLocalAddress()
{
    QHostAddress ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            return ipAddress = ipAddressesList.at(i);
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isNull())
        ipAddress = QHostAddress(QHostAddress::LocalHost);
    return ipAddress;
}

void TCPServer::newMember()
{
    if(newConnection = server->nextPendingConnection())
    {
        connect(newConnection, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
        emit addMember(newConnection);
    }
}

void TCPServer::emitDeleteMember()
{
    deleteMember(newConnection->peerAddress());
}

