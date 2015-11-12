#ifndef PORTS_H
#define PORTS_H

#define UDP_PORT 8534
#define TCP_PORT 8081

#define WAIT_FOR_CONN_TIME_MSEC 5000

// for controller
#define CHECK_ALIVE_TIMEOUT_MSEC 1000

// for udpService
#define SEND_ALIVE_TIMEOUT_MSEC 500

#include <QDateTime>
struct SharedFile
{
    QString name;
    QDateTime lastChange;
};

#include <QHostAddress>
struct RoomMember
{
    QString login;
    QHostAddress addr;
};

#endif // PORTS_H

