#include "common.h"

QDataStream &operator <<(QDataStream &s, const DatagramPacket &packet)
{
    s<<packet.type<<packet.id<<packet.content;
    return s;
}

QDataStream &operator >>(QDataStream &s, DatagramPacket &packet)
{
    s>>packet.type>>packet.id>>packet.content;
    return s;
}


bool operator<(const QHostAddress &first, const QHostAddress &second)
{
    return first.toIPv4Address()<second.toIPv4Address();
}
