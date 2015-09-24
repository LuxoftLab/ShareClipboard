#include "controller.h"
#include "gui/mainwindow.h"
#include <QDebug>

Controller::Controller(ClipboardTrayIcon * icon) : QObject(0)
{
    this->icon = icon;
    connect(icon, SIGNAL(roomListOpened(RoomsListDialog*)),
            this, SLOT(onRoomsListOpen(RoomsListDialog*)));
    connect(icon, SIGNAL(serverRoomCreated(QString,QString)),
            this, SLOT(createServerRoom(QString,QString)));
    connect(this, SIGNAL(serverIsUp(QString)),
            icon, SLOT(becomeServer(QString)));
    connect(icon, SIGNAL(deleteServerSignal()),
            this, SLOT(deleteServerRoom()));
    connect(this, SIGNAL(serverIsDown()),
            icon, SLOT(stopBeignServer()));
    connect(icon, SIGNAL(toggleSharingSignal()),
            &(this->clipboardService), SLOT(turnSharing()));
    icon->show();

    initClipboardToGuiConnection();
    initUDPService();
}

Controller::~Controller()
{
    delete icon;
    for(QMap<qint32, ClientRoom*>::iterator it = rooms.begin(); it != rooms.end(); ++it)
        delete it.value();
    delete udpService;
}

bool Controller::isServer()
{
    return this->server;
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
    qDebug() << "room deleted";
    QString name = rooms.value(host.toIPv4Address())->getName();
    rooms.remove(host.toIPv4Address());
    emit roomDeleted(name);
}

void Controller::createServerRoom(QString name, QString pass)
{
    if(serverRoom != NULL)
        return;

    qDebug() << "create server room: " << name;

    serverRoom = new ServerRoom(name, pass);
    udpService->notifyAboutRoom(name);
    emit serverIsUp(name);

    //addRoom(name, QHostAddress("127.0.0.1"));
}

void Controller::deleteServerRoom()
{
    if(serverRoom == NULL)
        return;
    deleteRoom(QHostAddress("127.0.0.1"));

    delete serverRoom;
    serverRoom = NULL;

    qDebug() << "server room deleted ";

    udpService->notifyAboutRoomDeleting();
    emit serverIsDown();
}

void Controller::createFloatingServerRoom(QHostAddress)
{
    deleteServerRoom();
    createServerRoom(clientRoom->getLogin(), clientRoom->getPwd());
    QString pwd = clientRoom->getPwd();
    delete clientRoom;
    clientRoom = NULL;
    joinRoom(QHostAddress("127.0.0.1").toIPv4Address(), pwd);
}

void Controller::onRoomsListOpen(RoomsListDialog * roomsDialog)
{
    connect(roomsDialog, SIGNAL(newRoomCreated(QString,QString)),
            this, SLOT(createServerRoom(QString,QString)));
    connect(this, SIGNAL(roomAdded(QString,qint32)),
            roomsDialog, SLOT(addRoom(QString,qint32)));

    connect(this, SIGNAL(serverIsUp(QString)),
            roomsDialog, SLOT(onServerIsUp(QString)));
    connect(roomsDialog, SIGNAL(roomChoosed(qint32, QString)),
            this, SLOT(joinRoom(qint32,QString)));

    connect(roomsDialog, SIGNAL(deleteServerRoom()),
            this, SLOT(deleteServerRoom()));
    connect(this, SIGNAL(roomDeleted(QString)),
            roomsDialog, SLOT(deleteRoom(QString)));
}

void Controller::fileNotification(QString fileName, QHostAddress address, QDateTime stamp)
{
    icon->showMessage(fileName, address.toString()+
                      " last changed on "+
                      stamp.date().toString()+" "+
                      stamp.time().toString());
    emit hasFileToText(fileName+address.toString()+stamp.date().toString()+stamp.time().toString(),0);
}

void Controller::initClipboardToGuiConnection()
{
    connect(&clipboardService, SIGNAL(hasDataToText(QString, qint32)), icon, SLOT(dataPushedToClipboard(QString, qint32)));
    connect(this, SIGNAL(hasFileToText(QString, qint32)),
            icon, SLOT(dataPushedToClipboard(QString, qint32)));
    connect(&clipboardService, SIGNAL(deleteDataFromStorage(qint32)), icon, SLOT(deleteItemFromList(qint32)));
    connect(icon, SIGNAL(pushDataChoosed(qint32)), &clipboardService, SLOT(pushDataToClipboardFromGui(qint32)));
    connect(icon, SIGNAL(settingsChoosed(int, bool)), &clipboardService, SLOT(onSettingsChoosed(int,bool)));
}

void Controller::initUDPService()
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

void Controller::joinRoom(qint32 addr, QString pass)
{
    QHostAddress host(addr);
    qDebug() << "joining room: " << host.toString();

    if(clientRoom != NULL)
        return;

    clientRoom = rooms.value(addr, NULL);
    if(clientRoom == NULL)
        return;
    qDebug() << "joined";

    QString login = "login";

    try{
    clientRoom->connectToHost(login, pass);
    }
    catch(QAbstractSocket::SocketError)
    {
        qDebug() << "Socket problem. You probably have your Wifi turned off";
        QMessageBox* networkerrormsg = new QMessageBox();
        networkerrormsg->setText("Socket problem. You probably have your Wifi turned off");
    }
    connect(&clipboardService, SIGNAL(hasData(QByteArray, QString)),
            clientRoom, SLOT(sendData(QByteArray, QString)));
    connect(clientRoom, SIGNAL(gotData(QByteArray, QString)),
            &clipboardService, SLOT(pushFromHosts(QByteArray,QString)));

    connect(clientRoom, SIGNAL(newFloatingServer(QHostAddress)),
            this, SLOT(createFloatingServerRoom(QHostAddress)));
    connect(clientRoom, SIGNAL(gotFileNotification(QString,QHostAddress,QDateTime)),
            this, SLOT(fileNotification(QString,QHostAddress,QDateTime)));
    connect(icon, SIGNAL(messageClicked()),
            clientRoom, SLOT(requestFile()));

    clientRoom->setLogin(host.toString());
    clientRoom->setPwd(pass);
}
