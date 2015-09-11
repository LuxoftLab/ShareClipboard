#ifndef UDP_PACKET_H
#define UDP_PACKET_H

#include <QDataStream>
#include "constants.h"

enum PacketType{
    ROOM = 0,
    GET_ROOM = 1,
    DELETE_ROOM = 2,
    UDP_IDLE
    };

struct DatagramPacket{
    quint8 type;
    qint32 id;
    QString name;
};

DatagramPacket unpackPackage(QDataStream& stream);

#endif
