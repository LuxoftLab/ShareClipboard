#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include <QTcpSocket>
#include <QMap>

#include "room.h"
#include "tcp_server.h"
#include "client_connection.h"
#include "constants.h"

struct FileWaitor
{
    SharedFile file;
    bool sent = false;
    ClientConnection * destination;
    FileWaitor(SharedFile, ClientConnection *);
};

class ServerRoom : public Room
{
    Q_OBJECT
    TCPServer * server;
    QMap<qint32, ClientConnection*> notVerified;
    QMap<qint32, ClientConnection*> verified;
    QMap<QString, ClientConnection*> fileMetaData;
    QList<FileWaitor> fileWaitors;

    void saveFileMetaData(QString, ClientConnection * const);
public:
    ServerRoom(QString name, QString pass);
    ~ServerRoom();
public slots:
    void addMember(qintptr socketDescriptor);
    void deleteMember(QHostAddress addr);
    bool verifyPass(QString pass, FloatServerPriority, ClientConnection * const);

    void onText(QString, ClientConnection * const);
    void onImage(QByteArray, ClientConnection * const);
    void onFileNotification(QString, QDateTime, ClientConnection * const);
    void onFileRequest(QString, QDateTime, ClientConnection * const);
    void onFileResponse(QString, QDateTime, QByteArray, ClientConnection * const);

    void getFile(QString);
    ClientConnection * getFileOwner(QString);
};

#endif // SERVER_ROOM_H
