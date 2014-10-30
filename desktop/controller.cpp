#include "controller.h"
#include <QDebug>
Controller::Controller() : QObject(0)
{
    udpService = new UDPService();
    connect(udpService, SIGNAL(roomReceived(QString,QHostAddress)),
            this, SLOT(addRoom(QString,QHostAddress)));
    connect(udpService, SIGNAL(roomRequested()),
            this, SLOT(getRoom()));
    connect(udpService, SIGNAL(roomDeleted(QHostAddress)),
            this, SLOT(deleteRoom(QHostAddress)));
    udpService->initListener();
    udpService->getRooms();
}

Controller::~Controller()
{
    delete udpService;
}

void Controller::addRoom(QString name, QHostAddress host)
{
    qDebug() << "new room: " << name << ' ' << host.toString();
    rooms.insert(host.toIPv4Address(), new ClientRoom(name, host));
}

void Controller::getRoom()
{
    qDebug() << "room requested";
    if(serverRoom != NULL) {
        udpService->sendRoom(serverRoom->getName());
    }
}

void Controller::deleteRoom(QHostAddress host)
{
    rooms.remove(host.toIPv4Address());
}

void Controller::createRoom(QString name, QString pass, QString login)
{
    qDebug() << "create room: " << name;
    if(serverRoom != NULL) {
        return;
    }
    serverRoom = new ServerRoom(name, pass);
    udpService->notifyAboutRoom(name);
    //addRoom(name, serverRoom->getAddr());
    //joinRoom(serverRoom->getAddr(), login, pass);
}

void Controller::joinRoom(qint32 addr, QString pass, QString login)
{
    qDebug() << "join room: " << addr;
    if(clientRoom != NULL) {
        return;
    }
    clientRoom = rooms.value(addr, NULL);
    if(clientRoom == NULL) {
        return;
    }
    clientRoom->connectToHost(login, pass);
}
