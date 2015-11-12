#include "udpservice.h"

UdpService::UdpService(QObject *parent)
{
    aliveTimer = new QTimer(this);
    connect(aliveTimer, &QTimer::timeout, this, &UdpService::iAmAlive);
    aliveTimer->start(SEND_ALIVE_TIMEOUT_MSEC);

    socket = new QUdpSocket(this);
    if(!socket->bind(UDP_PORT, QUdpSocket::ShareAddress)){
        throw 1; // ###
    }
    connect(socket, &QUdpSocket::readyRead, this, &UdpService::read);
}

void UdpService::iAmAlive()
{
        QByteArray broadcast = "Broadcast message!";
        socket->writeDatagram(broadcast, broadcast.size(), QHostAddress::Broadcast, UDP_PORT);
}

void UdpService::read()
{
    while (socket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size());
            qDebug() << datagram;
        }
}

void UdpService::run()
{
    this->read();
}
