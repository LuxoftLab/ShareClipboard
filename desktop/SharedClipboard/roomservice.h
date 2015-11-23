#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QNetworkInterface>
#include <QByteArray>
#include <QTimer>
#include <QList>


#include "udpservice.h"
#include "tcpservice.h"
#include "shared_clipboard_exception.h" //###


class RoomService : public QObject
{
    Q_OBJECT

    QSharedPointer<UdpService> udpService;
    QSharedPointer<TcpService> tcpService;

    QList<RoomMember> roomMembers;
    QList<QString> roomsNames;

    QString login;
    QString ownRoomName;
    QTimer * checkAlivesTimer;
public:
    explicit RoomService(QString & login, QObject *parent = 0);
    QList<QString> getRooms();
signals:

public slots:
    void sendData(TcpPackage, QByteArray&);
    void reveiveData(TcpPackage, QByteArray&);

    void addMember(QString login, QString room, QList<QHostAddress> ip);
    void checkAlives();
    void setRoom(QString &);
};

#endif // CONTROL_H
