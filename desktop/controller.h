#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QHostAddress>
#include <QVector>

#include "udp_service.h"
#include "server_room.h"
#include "client_room.h"

class UDPService;
class Controller
{
    UDPService* udpService;
    ServerRoom* serverRoom;
    ClientRoom* clientRoom;
    QVector<ClientRoom*> rooms;
public:
    Controller();
    ~Controller();
    void addRoom(QString name, QHostAddress host);
    bool createRoom(QString name, QString pass);
    bool joinRoom(int index);
};

#endif // CONTROLLER_H
