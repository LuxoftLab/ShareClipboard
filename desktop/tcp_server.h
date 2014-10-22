#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "server_room.h"
class ServerRoom;
class TCPServer
{
    ServerRoom * room;
public:
    TCPServer(ServerRoom * room);
};

#endif // TCP_SERVER_H
