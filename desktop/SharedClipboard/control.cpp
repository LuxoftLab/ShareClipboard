#include "control.h"

Control::Control(QString & value, QObject *parent) : QObject(parent)
{
    login = value;
    udpService = QSharedPointer<UdpService>(new UdpService(login, this));
    connect(udpService.data(), &UdpService::newMember,
            // add roomname to roomslist, if not already in it, add member if it is in our room list and is not yet there
            [=](QString l, QString r, QList<QHostAddress> addrl){
                if(!roomsNames.contains(r)){
                    roomsNames.append(r);
                }

                if(r != this->ownRoomName)
                    return;
                RoomMember candidate = RoomMember(l, addrl, true, 0);
                if(roomMembers.contains(candidate)){
                    RoomMember present = roomMembers.at(roomMembers.lastIndexOf(candidate));
                    present.alive = true;
                    present.packs_count = 0;
                }
                roomMembers.append(RoomMember(l, addrl, true, 0));
            });

    // start sniffing for other members over udp
    udpService->start();

    checkAlivesTimer = new QTimer(this);
    connect(checkAlivesTimer, &QTimer::timeout, this, &Control::checkAlives);
    checkAlivesTimer->start(CHECK_ALIVE_TIMEOUT_MSEC);

    tcpService = QSharedPointer<TcpService>(new TcpService(this));
    tcpService->createServer();
}

void Control::sendData(TcpPackage type, QByteArray & data)
{
    tcpService->sendData(type, data);
}

void Control::reveiveData(TcpPackage, QByteArray &)
{

}

void Control::addMember(QString, QHostAddress)
{

}

void Control::checkAlives()
{
    for (auto member : roomMembers){
        if(member.packs_count < MIN_RESP_COUNT)
            member.packs_count = 0;
            member.alive = false;
    }

}



