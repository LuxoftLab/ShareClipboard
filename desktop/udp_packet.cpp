#include "udp_packet.h"

DatagramPacket unpackPackage(QDataStream &stream){
    DatagramPacket packet;
    stream >> packet.type;
    stream >> packet.id;

    if(packet.type == ROOM){
        char* fromBytes;
        stream >> fromBytes;
        packet.name = QString::fromUtf8(fromBytes);
    }

    return packet;
}
