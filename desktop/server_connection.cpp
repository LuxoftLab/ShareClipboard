#include "server_connection.h"
#include <unistd.h>
ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this); // is socket already initialised in parent class?
    socket->connectToHost(host, (qint16)getpid()); // I don't know what else to do here
    if(!socket->ConnectedState)

}

void ServerConnection::sendPassAndLogin(QString password, QString login)
{

}
