#ifndef COMMON_H
#define COMMON_H

#include <QDataStream>
#include <QString>

#define UDP_PORT 1234
#define REPEAT 5

namespace PacketType
{
    enum pkt_type{Invalid=0,Lookup=1,Hello=2,Clipboard=4,AreYouHere=8};
}

struct DatagramPacket
{
    qint32 type;
    qint32 length;
    QString content;
};

QDataStream & operator <<(QDataStream & s,const DatagramPacket & packet);
QDataStream & operator >>(QDataStream & s,DatagramPacket & packet);

#endif // COMMON_H
