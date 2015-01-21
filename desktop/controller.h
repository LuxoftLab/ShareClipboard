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
    QMap <qint32, ClientRoom*> rooms;
public:
    Controller();
    ~Controller();
signals:
    void roomAdded(QString name, qint32 ip);
    void roomDeleted(QString name);
    void serverIsUp(QString name);

public slots:
    void createServerRoom(QString name, QString pass);
    void joinRoom(QHostAddress addr, QString login, QString pass);
    void getRoom(QHostAddress sender_addr);
    void addRoom(QString name, QHostAddress host);
    void deleteRoom(QHostAddress host);
    void deleteServerRoom();
};

#endif // CONTROLLER_H
