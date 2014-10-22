#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include <QTcpSocket>
#include <QVector>

#include "room.h"
#include "tcp_server.h"
#include "client_connection.h"

class ServerRoom : public Room
{
    TCPServer * server;
    QVector<ClientConnection*> notVerified;
    QVector<ClientConnection*> verified;
public:
    ServerRoom(QString name, QString pass);
    ~ServerRoom();
public slots:
    void addMember(QTcpSocket * socket);
    bool verifyPass(QString pass, ClientConnection& conn);
};

#endif // SERVER_ROOM_H
