#ifndef CLIENT_ROOM_H
#define CLIENT_ROOM_H

#include <QHostAddress>
#include <QMap>
#include <QImage>
#include <QBuffer>

#include "room.h"
#include "server_connection.h"

struct Member {
    QString login;
    QHostAddress addr;
    floating_server_priorities priority;
    Member(QString login, QHostAddress addr, floating_server_priorities prior);
};

class ClientRoom : public Room
{
    Q_OBJECT
    ServerConnection * connection;
    QHostAddress host;
    QMap<qint32, Member*> members;

    floating_server_priorities device_type();
public:
    ClientRoom(QString name, QHostAddress host);
    ~ClientRoom();
    void connectToHost(QString login, QString pass);
public slots:
    void addMember(QString login, floating_server_priorities, QHostAddress addr);
    void deleteMember(QHostAddress addr);
    void sendData(QByteArray data, QString type);
    void recoverServer();
//    void sendText(QString text);
//    void sendImage(QImage);
//    void updateBuffer();
//    void setNotUpdated();
signals:
    void gotData(QByteArray, QString);
    void gotText(QString);
    void gotImage(QByteArray);
};

#endif // CLIENT_ROOM_H
