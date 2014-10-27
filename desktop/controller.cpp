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

bool Controller::createRoom(QString name, QString login, QString pass)
{
    if(serverRoom != NULL) {
        return false;
    }
    serverRoom = new ServerRoom(name, pass);
    udpService->notifyAboutRoom(name);
    addRoom(name, serverRoom->getAddr());
    joinRoom(serverRoom->getAddr(), login, pass);
    return true;
}

bool Controller::joinRoom(QHostAddress host, QString login, QString pass)
{
    if(clientRoom != NULL) {
        return false;
    }
    clientRoom = rooms.value(host.toIPv4Address(), NULL);
    if(clientRoom == NULL) {
        return false;
    }
    clientRoom->connectToHost(login, pass);
    return true;
}
