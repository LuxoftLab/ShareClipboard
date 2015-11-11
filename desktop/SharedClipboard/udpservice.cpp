#include "udpservice.h"

UdpService::UdpService(QObject *parent) : QObject(parent)
{

}

void UdpService::iAmAlive()
{

}

void UdpService::read()
{

}

void UdpService::run()
{
    this->read();
}
