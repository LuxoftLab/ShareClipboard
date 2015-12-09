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

    QString ownRoomName;
    QTimer * checkAlivesTimer;
public:
    explicit RoomService(QObject *parent = 0);
    QList<QString> getRooms();
    QList<RoomMember> getRoomMembers() const;

signals:
    void refreshMembers(QList<RoomMember>);

public slots:
    void addMember(QString login, QString room, QList<QHostAddress> ip);
    void checkAlives();
    void setRoom(QString &);
};

#endif // CONTROL_H
