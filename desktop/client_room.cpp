#include "client_room.h"

ClientRoom::ClientRoom(QString name, QHostAddress host) : Room(name, "")
{
    this->host = host;
}

void ClientRoom::connectToHost()
{

}

void ClientRoom::addMember(QString login, QHostAddress addr) {

}

ClientRoom::~ClientRoom() {

}
