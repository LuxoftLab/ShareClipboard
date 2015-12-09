#include "sessionmanager.h"

QString SessionManager::sharedClipboardIdFileName = QString::fromUtf8(".SharedClipboard/.config");
QString SessionManager::idFilePath = QDir::homePath() + "/" + sharedClipboardIdFileName;

SessionManager::SessionManager(){}


bool SessionManager::idFileExists()
{
    QFile idFile(idFilePath);
    if(idFile.exists()){
        return true;
    } else {
        return false;
    }
}

bool SessionManager::idFileOpenedForWriting()
{
    QFile file(idFilePath);
    bool ready = file.open(QIODevice::WriteOnly);
    file.close();
    return !ready;
}


QString SessionManager::getLogin(QFile source)
{
    if(idFileExists() && source.open(QIODevice::ReadOnly)){
            QDataStream idFileStream(&source);
            QString login;
            idFileStream >> login;
            return login;
    }
    return nullptr;
}

SessionManager * SessionManager::getInstance()
{
    return new SessionManager();
}

bool SessionManager::sessionActive()
{
    if(idFileExists() && idFileOpenedForWriting())
        return true;
    return false;
}

bool SessionManager::sessionInterrupted()
{
    if(idFileExists() && !idFileOpenedForWriting())
        return true;
    return false;
}

void SessionManager::startSession(QSharedPointer<RoomService> &roomService,
                                  QSharedPointer<UdpService> &udpService,
                                  QSharedPointer<TcpService> &tcpService,
                                  QSharedPointer<ClipboardService> &clipboardService,
                                  QSharedPointer<FileService> &fileService,
                                  QSharedPointer<EncryptionService> &encService)
{
    LoginPassDialog * dlg = new LoginPassDialog();
    QObject::connect(dlg, &LoginPassDialog::credentials,
                     [&](QString l, QString p){
                        QString testLogin = l;
                        QString pass = p;

                        udpService = QSharedPointer<UdpService>(new UdpService(testLogin, NULL));

                        QObject::connect(udpService.data(), &UdpService::newMember,
                             roomService.data(), &RoomService::addMember);

                        // start sniffing for other members over udp
                        udpService->start();

                        tcpService = QSharedPointer<TcpService>(new TcpService());

                        QObject::connect(roomService.data(), &RoomService::refreshMembers,
                                         tcpService.data(), &TcpService::setRoomMembers);
                        tcpService->createServer();

                        encService = QSharedPointer<EncryptionService>(new EncryptionService(pass));
                        clipboardService = QSharedPointer<ClipboardService>(new ClipboardService());
                        QObject::connect(tcpService.data(), &TcpService::gotData,
                                         clipboardService.data(), &ClipboardService::updateClipboard);
                        QObject::connect(clipboardService.data(), &ClipboardService::clipboardChanged,
                                         tcpService.data(), &TcpService::send);
                        });

    roomService = QSharedPointer<RoomService>(new RoomService(NULL));
    QList<QString> room_list = roomService->getRooms();
    RoomChoose * roomChoose = new RoomChoose(room_list);
    QObject::connect(roomChoose, &RoomChoose::room, [=](QString roomname){
                            QString room = roomname;
                            roomService->setRoom(room);
                            udpService->setRoomName(room);
                        });
    roomChoose->show();

    dlg->show();
    delete dlg;
    delete roomChoose;
}

