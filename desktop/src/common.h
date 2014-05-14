#ifndef COMMON_H
#define COMMON_H

#include <QDataStream>
#include <QString>
#include <QHostAddress>

#define UDP_PORT 1234
#define PEER_CHECK_TIMEOUT 45000
#define PEER_OFFLINE_TIMEOUT 60000
#define PEER_TIMER_TIMEOUT  1000
#define REPEAT 5

namespace PacketType
{
    enum pkt_type{Invalid=0,Lookup=1,Hello=2,Clipboard=4,AreYouHere=8};
}

struct DatagramPacket
{
    qint32 type;
    qint32 id;
    QString content;
};

QDataStream & operator <<(QDataStream & s,const DatagramPacket & packet);
QDataStream & operator >>(QDataStream & s,DatagramPacket & packet);

bool operator<(const QHostAddress & first, const QHostAddress & second);

#endif // COMMON_H
