#include "controller.h"

Controller::Controller() : QObject(0)
{
    udpService = new UDPService();
    connect(udpService, SIGNAL(UDPService::addRoom()), this, SLOT(addRoom()));
    udpService->initListener();
    udpService->getRooms();
}

Controller::~Controller()
{
    delete udpService;
}

void Controller::addRoom(QString name, QHostAddress host)
{
    rooms.insert(host.toIPv4Address(), new ClientRoom(name, host));
}

void Controller::getRoom()
{
    if(serverRoom != NULL) {
        udpService->sendRoom(serverRoom->getName());
    }
}

void Controller::deleteRoom(QHostAddress host)
{
    rooms.remove(host.toIPv4Address());
}

bool Controller::createRoom(QString name, QString pass)
{
    if(serverRoom != NULL) {
        return false;
    }
    serverRoom = new ServerRoom(name, pass);
    udpService->notifyAboutRoom(name);
    addRoom(name, serverRoom->getAddr());
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
