#include "common.h"

QDataStream &operator <<(QDataStream &s, const DatagramPacket &packet)
{
    s<<packet.type<<packet.length<<packet.content;
    return s;
}

QDataStream &operator >>(QDataStream &s, DatagramPacket &packet)
{
    s>>packet.type>>packet.length>>packet.content;
    return s;
}
