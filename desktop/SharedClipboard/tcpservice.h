#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QSharedPointer>
#include <QThread>

#include "tcppackages.h"
#include "common.h"
#include "encryptionservice.h"

class TcpService : public QObject
{
    Q_OBJECT

    QTcpServer * ownServer = NULL;
    QList<RoomMember> roomMembers;
    QList<QTcpSocket*> roomSockets; // came over udp, to send to
    QList<QTcpSocket*> connectedSockets; // connected to my server, to receive from
    QList<QTcpSocket*> fileSockets; // to send files

    void gotFiles();

public:
    explicit TcpService(QObject *parent = 0);

    void createServer();

    void connectSocket(QTcpSocket*, QHostAddress dest);
    void addRoomMembers(QList<QHostAddress>);

    void send(TcpPackage type, QByteArray&);
signals:
    void gotData(TcpPackage, QByteArray &);

public slots:
    void setRoomMembers(QList<RoomMember>);
private slots:
    void read();
    void registerConnection();
};

#endif // TCPSERVICE_H
