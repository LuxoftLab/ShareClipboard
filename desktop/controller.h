#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDebug>

#include <QObject>
#include <QHostAddress>
#include <QVector>
#include <QMap>

#include "udp_service.h"
#include "server_room.h"
#include "client_room.h"

class Controller : public QObject
{
    Q_OBJECT
private:
    UDPService* udpService = NULL;
    ServerRoom* serverRoom = NULL;
    ClientRoom* clientRoom = NULL;
    QMap<qint32, ClientRoom*> rooms;
public:
    Controller();
    ~Controller();
public slots:
    void createRoom(QString name, QString pass, QString login);
    void joinRoom(qint32 addr, QString pass, QString login);
    void getRoom();
    void addRoom(QString name, QHostAddress host);
    void deleteRoom(QHostAddress host);
};

#endif // CONTROLLER_H
