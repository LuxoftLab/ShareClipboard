#ifndef PORTS_H
#define PORTS_H

#define UDP_PORT 85347
#define TCP_PORT 8081

#define WAIT_FOR_CONN_TIME_MSEC 5000


#include <QDateTime>
struct SharedFile
{
    QString name;
    QDateTime lastChange;
};

#include <QTcpSocket>
struct RoomMember
{
    QString login;
    QTcpSocket * socket;
};

#endif // PORTS_H

