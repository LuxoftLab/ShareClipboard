#ifndef UDP_SERVICE_H
#define UDP_SERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QTime>
#include <QTimer>
#include <QBuffer>
#include <QVector>
#include <QDataStream>
#include <QTcpSocket>

#include "udp_packet.h"

class UDPService : public QObject {
    Q_OBJECT

public:
    UDPService();
    bool initListener();
    void getRooms();
    void sendRoom(QString name, QHostAddress addr);
    void notifyAboutRoom(QString name);
    void notifyAboutRoomDeleting();
signals:
    void roomReceived(QString name, QHostAddress host);
    void roomDeleted(QHostAddress host);
    void roomRequested(QHostAddress sender_address);
private slots:
    void clearReceivedId();
private:
    void sendPackage(const QHostAddress &peer, const DatagramPacket &packet);
    void sendBroadcastPackadge(const DatagramPacket &packet);
    void listener();

    QVector <QHostAddress> localhost_ip;
    QVector <int> received_id;
    QUdpSocket * udp_socket;
    QList <QHostAddress> broadcasts;
};

#endif // UDP_SERVICE_H
