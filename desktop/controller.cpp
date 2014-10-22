#include "controller.h"

Controller::Controller() : QObject(0)
{
    udpService = new UDPService();
    connect(udpService, SIGNAL(UDPService::addRoom()), this, SLOT(addRoom()));
}

Controller::~Controller()
{
    delete udpService;
}

void Controller::addRoom(QString name, QHostAddress host)
{

}

bool Controller::createRoom(QString name, QString pass)
{
    return false;
}

bool Controller::joinRoom(int index)
{
    return false;
}
