#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "roomservice.h"
#include "clipboardservice.h"
#include "fileservice.h"
#include "GUI/loginpassdialog.h"
#include "GUI/roomchoose.h"

class SessionManager
{
    SessionManager();

    LoginPassDialog * dlg;
public:
    static QString sharedClipboardIdFileName;
    static QString idFilePath;

    static bool sessionActive();
    static bool sessionInterrupted();
    void startSession( QSharedPointer<RoomService> & roomService,
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
