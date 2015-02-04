
#include "controller.h"
#include "gui/mainwindow.h"
#include <QDebug>

Controller::Controller(MainWindow *mainWindow) : QObject(0)
{
    this->mainWindow = mainWindow;
    connect(mainWindow, SIGNAL(roomListOpened(RoomsListDialog*)), this, SLOT(onRoomsListOpen(RoomsListDialog*)));

    initClipboardToGuiConnection();
    initUDPService();
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

    //addRoom(name, serverRoom->getAddr());

    qDebug() << serverRoom->getAddr();

    QString login = "login";
    //joinRoom(serverRoom->getAddr().toIPv4Address(), pass);
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

void Controller::onRoomsListOpen(RoomsListDialog * roomsDialog)
{
    QMap<QString, qint32> roomsForGui;
    QMapIterator<qint32, ClientRoom*> i(rooms);
    while (i.hasNext())
    {
        i.next();
        roomsForGui.insert(i.value()->getName(), i.key());
    }

    if(serverRoom == NULL)
    {
        roomsDialog->setRoomsHash(roomsForGui, QString::null);
    } else {
        roomsDialog->setRoomsHash(roomsForGui, serverRoom->getName());
    }

    connect(roomsDialog, SIGNAL(newRoomCreated(QString,QString)), this, SLOT(createServerRoom(QString,QString)));
    connect(this, SIGNAL(roomAdded(QString,qint32)), roomsDialog, SLOT(addRoom(QString,qint32)));

    connect(this, SIGNAL(serverIsUp(QString)), roomsDialog, SLOT(onServerIsUp(QString)));
    connect(roomsDialog, SIGNAL(roomChoosed(qint32, QString)), this, SLOT(joinRoom(qint32,QString)));

    connect(roomsDialog, SIGNAL(deleteServerRoom()), this, SLOT(deleteServerRoom()));
    connect(this, SIGNAL(roomDeleted(QString)), roomsDialog, SLOT(deleteRoom(QString)));

}

void Controller::initClipboardToGuiConnection()
{
    connect(&clipboardService, SIGNAL(hasText(qint32, QString)), mainWindow, SLOT(textPushedToClipboard(qint32, QString)));
    connect(&clipboardService, SIGNAL(deleteDataFromStorage(qint32)), mainWindow, SLOT(deleteItemFromList(qint32)));

   // connect(&clipboardService, SIGNAL(hasImage(QPixmap)), mainWindow, SLOT(imagePushedToClipboard(QPixmap)));
   // connect(&clipboardService, SIGNAL(hasImage(QString)), mainWindow, SLOT(imagePushedToClipboard(QString)));
    connect(mainWindow, SIGNAL(pushDataChoosed(qint32)), &clipboardService, SLOT(pushDataToClipboard(qint32)));

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
    //QObject::connect(&cservice, SIGNAL(pasteText(QString)), clientRoom, SLOT(sendText(QString)));
    if(clientRoom == NULL)
        return;
    qDebug() << "joined";

    QString login = "login";
    clientRoom->connectToHost(login, pass);

    MainWindow().show();
}
