#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include <QUdpSocket>
#include <QVector>

#include "room.h"
#include "tcp_server.h"
#include "client_connection.h"

class ClientConnection;
class ServerRoom : public Room
{
    QVector<ClientConnection*> notVerified;
    QVector<ClientConnection*> verified;
public:
    ServerRoom(QString name, QString pass);
    ~ServerRoom();
    void addMember(QUdpSocket socket);
    bool verifyPass(QString pass, ClientConnection& conn);
};

#endif // SERVER_ROOM_H
