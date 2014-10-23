#ifndef UDP_SERVICE_H
#define UDP_SERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QBuffer>
#include <QList>
#include <QDataStream>
#include <QNetworkInterface>
#include <QTime>

#include "UDP_packet.h"

class UDPService : public QObject
{
    Q_OBJECT

public:
    UDPService();
    bool initListener();
    void getRooms();
    void sendRoom(QString name);
    void notifyAboutRoom(QString name);
signals:
    void addRoom(QString name, QHostAddress host);
private:
    void sendPackage(const QHostAddress &peer, const DatagramPacket &packet);
    void sendBroadcastPackadge(int type, QString room_name);
    void listener();

    QHostAddress localhost_ip;
    QUdpSocket * udp_socket;
    QList<QHostAddress> broadcasts;
    QList<QHostAddress> senders;
    qint32 last_packadge_id;
};

#endif // UDP_SERVICE_H
