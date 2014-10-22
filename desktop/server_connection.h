#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "connection.h"
#include "client_room.h"

class ServerConnection : public Connection
{
    ClientRoom * room;
public:
    ServerConnection(ClientRoom * room);
    void sendPassAndLogin(QString password, QString login);
};

#endif // SERVER_CONNECTION_H
