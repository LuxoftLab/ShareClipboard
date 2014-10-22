#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H


#include "connection.h"
#include "server_room.h"
class ServerRoom;
class ClientConnection : public Connection
{
    QString login;
    ServerRoom * room;
public:
    ClientConnection(ServerRoom * room, QUdpSocket * socket);
    void sendFail();
    void sendMember(QString login, quint32 ipv4);
    QString getLogin();
};

#endif // CLIENT_CONNECTION_H
