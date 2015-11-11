#include "control.h"

Control::Control(QString & value, QObject *parent) : QObject(parent)
{
    login = value;
    udpService = QSharedPointer<UdpService>(new UdpService());
    // start sniffing for other members over udp
    udpService->run();
    tcpService = QSharedPointer<TcpService>(new TcpService());

    tcpService->createServer();
}

void Control::sendData(TcpPackage type, QByteArray & data)
{
    tcpService->sendData(type, data);
}



