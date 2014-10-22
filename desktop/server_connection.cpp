#include "server_connection.h"

ServerConnection::ServerConnection(ClientRoom * room) : Connection(NULL)
{
    this->room = room;
}

void ServerConnection::sendPassAndLogin(QString password, QString login)
{

}
