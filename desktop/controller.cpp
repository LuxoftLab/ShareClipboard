#include "controller.h"
#include <QDebug>

Controller::Controller() : QObject(0)
{
    udpService = new UDPService();
    connect(udpService, SIGNAL(roomReceived(QString,QHostAddress)),
            this, SLOT(addRoom(QString,QHostAddress)));
    connect(udpService, SIGNAL(roomRequested(QHostAddress)),
            this, SLOT(getRoom(QHostAddress)));
    connect(udpService, SIGNAL(roomDeleted(QHostAddress)),
            this, SLOT(deleteRoom(QHostAddress)));

    udpService->initListener();
    udpService->getRooms();
}

Controller::~Controller()
{
    delete udpService;
}

void Controller::addRoom(QString name, QHostAddress host)
{
    qDebug() << "new room: " << name << ' ' << host.toString();
    ClientRoom *last = rooms.value(host.toIPv4Address(), NULL);
    if(last == NULL) {
        rooms.insert(host.toIPv4Address(), new ClientRoom(name, host));
        emit roomAdded(name, host.toIPv4Address());
    }
}

void Controller::getRoom(QHostAddress sender_address)
{
    qDebug() << "room requested";
    if(serverRoom != NULL)
        udpService->sendRoom(serverRoom->getName(), sender_address);

}

void Controller::deleteRoom(QHostAddress host)
{
<<<<<<< HEAD
=======
    qDebug() << "room deleted";
    QString name = rooms.value(host.toIPv4Address())->getName();
>>>>>>> 1c0c7652828a54eb7bc206825d9f34957b2c0a05
    rooms.remove(host.toIPv4Address());
}

void Controller::createServerRoom(QString name, QString pass)
{
    qDebug() << serverRoom;

    if(serverRoom != NULL)
        return;

    qDebug() << "create server room: " << name;

    serverRoom = new ServerRoom(name, pass);
    udpService->notifyAboutRoom(name);
    emit serverIsUp(name);

    //addRoom(name, serverRoom->getAddr());

    //QString login = "login";
    //joinRoom(serverRoom->getAddr(), login, pass);
}

void Controller::deleteServerRoom()
{
    if(serverRoom == NULL)
        return;

    delete serverRoom;
    serverRoom = NULL;

    qDebug() << "server room deleted ";

    udpService->notifyAboutRoomDeleting();

}

void Controller::joinRoom(qint32 addr, QString pass)
{
    QHostAddress host(addr);
    qDebug() << "joining room: " << host.toString();

    if(clientRoom != NULL)
        return;

    clientRoom = rooms.value(addr, NULL);

    if(clientRoom == NULL)
        return;
<<<<<<< HEAD
    }
    qDebug() << "joined";
    clientRoom->connectToHost(login, pass);
=======

    //qDebug() << "joined";

    //QString login = "login";
    //clientRoom->connectToHost(login, pass);
>>>>>>> 1c0c7652828a54eb7bc206825d9f34957b2c0a05
}
