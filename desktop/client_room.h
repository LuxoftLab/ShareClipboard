#ifndef CLIENT_ROOM_H
#define CLIENT_ROOM_H

#include <QHostAddress>
#include <QVector>

#include "room.h"

struct Member {
    QString login;
    QHostAddress addr;
};

class ClientRoom : public Room
{
    QHostAddress host;
    QVector<Member*> members;
public:
    ClientRoom(QString name, QHostAddress host);
    ~ClientRoom();
    void connectToHost();
    void addMember(QString login, QHostAddress addr);
};

#endif // CLIENT_ROOM_H
