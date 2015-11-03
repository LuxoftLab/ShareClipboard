#include "udpservice.h"

UdpService::UdpService(QObject *parent) : QObject(parent)
{

}

QHostAddress UdpService::requestRoom()
{
    // temporary
    return QHostAddress("127.0.0.1");
}

