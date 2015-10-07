#ifndef CLIENT_ROOM_H
#define CLIENT_ROOM_H

#include <QHostAddress>
#include <QMap>
#include <QList>
#include <QQueue>
#include <QImage>
#include <QBuffer>
#include <QDateTime>
#include <QFileInfo>
#include <QFileDialog>

#include "room.h"
#include "constants.h"
#include "server_connection.h"

struct Member {
    QHostAddress addr;
    FloatServerPriority priority;
    Member(QHostAddress addr, FloatServerPriority prior);
};

//Q_DECLARE_METATYPE(std::unique_ptr<ServerConnection>)

class ClientRoom : public Room
{
    Q_OBJECT
    ServerConnection * connection;
    //std::unique_ptr<ServerConnection> connection;
    QHostAddress host;
    QHostAddress ownAdress;
    QMap<qint32, Member*> members;
    QQueue<const SharedFile *> files;
    QQueue<Member*> floating_server_candidates; //todo a priority queue ?
    QString login;
    QString pwd;

    FloatServerPriority device_type();
public:
    ClientRoom(QString name, QHostAddress host);
    ~ClientRoom();
    void connectToHost(QString login, QString pass);
    void sendFileRequest(QString name, QDateTime timeStamp);

    QString getLogin() const;
    void setLogin(const QString &value);

    QString getPwd() const;
    void setPwd(const QString &value);

public slots:
    void addMember(FloatServerPriority, QHostAddress addr);
    void deleteMember(QHostAddress addr);
    void sendData(QByteArray data, QString type);
    void recoverServer();
    void fileNotification(QString name);
    void sendrequestFile(int);
    //void requestFile(int index);
    void addFile(QString,QDateTime);
    void respondWithFile(QString, QDateTime);
    void saveSharedFile(QString, QDateTime, QByteArray);

signals:
    void gotData(QByteArray, QString);
    void gotText(QString);
    void gotImage(QByteArray);
    void gotFileNotification(QString,QDateTime);
    void notificateAboutFile(QString,QDateTime,int);
    void newFloatingServer(QHostAddress);

};

#endif // CLIENT_ROOM_H
