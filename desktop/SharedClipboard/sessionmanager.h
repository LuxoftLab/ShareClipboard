#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QDataStream>

#include "roomservice.h"
#include "clipboardservice.h"
#include "fileservice.h"

class SessionManager
{
    SessionManager();
public:
    static QString sharedClipboardIdFileName;
    static QString idFilePath;

    static bool sessionActive();
    static bool sessionInterrupted();
    static void startSession( QSharedPointer<RoomService> & roomService,
                            QSharedPointer<UdpService> & udpService,
                            QSharedPointer<TcpService> & tcpService,
                            QSharedPointer<ClipboardService> & clipboardService,
                            QSharedPointer<FileService> & fileService,
                            QSharedPointer<EncryptionService> & encService);

    static bool idFileExists();
    static bool idFileOpenedForWriting();

    static QString getLogin(QFile source);
    static QString setLogin(QString login);
    static SessionManager * getInstance();
};



#endif // SESSIONMANAGER_H
