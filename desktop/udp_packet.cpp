#include "udp_packet.h"

DatagramPacket unpackPackage(QDataStream &stream){
    DatagramPacket packet;
    stream >> packet.type;
    stream >> packet.id;
    int length;
    stream >> length;

    if(packet.type == ROOM){
        char * fromBytes = new char[length];
        stream.readRawData(fromBytes, length);
        packet.name = QString::fromUtf8(fromBytes, length);
    }

    return packet;
}
