#include "server_room.h"

ServerRoom::ServerRoom(QString name, QString pass) : Room(name, pass)
{
}

ServerRoom::~ServerRoom()
{

}

void ServerRoom::addMember(QTcpSocket * socket)
{

}

bool ServerRoom::verifyPass(QString pass, ClientConnection &conn)
{

}



