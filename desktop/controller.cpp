#include "controller.h"

Controller::Controller()
{
    udpService = new UDPService(this);
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
