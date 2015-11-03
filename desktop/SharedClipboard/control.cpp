#include "control.h"

Control::Control(QString l, QObject *parent) throw(WrongPasswordError) : QObject(parent)
{
    login = l;
    udpService = QSharedPointer<UdpService>(new UdpService());
    tcpService = QSharedPointer<TcpService>(new TcpService());

    roomHead = udpService->requestRoom();
    tcpService->createServer();

    if(!roomHead.isNull()){
        try
        {
            joinRoom();
        }
        catch(WrongPasswordError)
        {
            throw; //###
        }
    } else {
        createRoom();
    }
}

void Control::createRoom()
{
    qDebug() << "created room";
}

void Control::joinRoom()
{
    tcpService->connectSocket(roomHead);
    tcpService->authenticate(login);
    qDebug() << "joined room";
}

void Control::sendData(TcpPackage type, QByteArray & data)
{
    tcpService->sendData(type, data);
}

