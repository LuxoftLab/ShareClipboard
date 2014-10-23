#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include <QTcpSocket>
#include <QMap>

#include "room.h"
#include "tcp_server.h"
#include "client_connection.h"

class ServerRoom : public Room
{
    TCPServer * server;
    QMap<qint32, ClientConnection*> notVerified;
    QMap<qint32, ClientConnection*> verified;
public:
    ServerRoom(QString name, QString pass);
    ~ServerRoom();
    QHostAddress getAddr();
public slots:
    void addMember(QTcpSocket * socket);
    void deleteMember(QHostAddress addr);
    bool verifyPass(QString pass, ClientConnection * conn);
};

#endif // SERVER_ROOM_H
