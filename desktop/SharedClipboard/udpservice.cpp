#include "udpservice.h"

UdpService::UdpService(QString login, QObject *parent)
{
    this->login = login;
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
    //refactor into package
    QByteArray broadcast;
    QDataStream out(&broadcast, QIODevice::WriteOnly);
    QNetworkInterface * networkInterface = new QNetworkInterface();
    QList<QHostAddress> ownAddressList = networkInterface->allAddresses();

    // login.size-login-roomName.size-roomName-hostAddress.count-hostAddress.asInt
    out << login.toUtf8().size();
    out.writeRawData(login.toUtf8().constData(), login.toUtf8().size());
    out << roomName.size();
    out.writeRawData(roomName.toUtf8().constData(),roomName.toUtf8().size());
    out << ownAddressList.size();
    for(auto address : ownAddressList){
        out << address.toIPv4Address();
    }

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
