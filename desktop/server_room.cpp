#include "server_room.h"

ServerRoom::ServerRoom(QString name, QString pass) : Room(name, pass)
{
}

ServerRoom::~ServerRoom()
{

}

void ServerRoom::addMember(QUdpSocket socket)
{

}

bool ServerRoom::verifyPass(QString pass, ClientConnection &conn)
{

}



