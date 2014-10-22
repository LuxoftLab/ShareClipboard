#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QHostAddress>
#include <QVector>

#include "udp_service.h"
#include "server_room.h"
#include "client_room.h"

class Controller : public QObject
{
    Q_OBJECT
private:
    UDPService* udpService;
    ServerRoom* serverRoom;
    ClientRoom* clientRoom;
    QVector<ClientRoom*> rooms;
public:
    Controller();
    ~Controller();
    bool createRoom(QString name, QString pass);
    bool joinRoom(int index);
public slots:
    void addRoom(QString name, QHostAddress host);
};

#endif // CONTROLLER_H
