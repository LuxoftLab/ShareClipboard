#ifndef CLIENT_ROOM_H
#define CLIENT_ROOM_H

#include <QHostAddress>
#include <QMap>

#include "room.h"
#include "server_connection.h"

struct Member {
    QString login;
    QHostAddress addr;
    Member(QString login, QHostAddress addr);
};

class ClientRoom : public Room
{
    Q_OBJECT

    ServerConnection * connection;
    QHostAddress host;
    QMap<qint32, Member*> members;
public:
    ClientRoom(QString name, QHostAddress host);
    ~ClientRoom();
    void connectToHost(QString login, QString pass);
public slots:
    void addMember(QString login, QHostAddress addr);
    void deleteMember(QHostAddress addr);
    void sendText(QString text);
};

#endif // CLIENT_ROOM_H
