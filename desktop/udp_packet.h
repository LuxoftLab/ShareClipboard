#ifndef PACKET_H
#define PACKET_H

#define REPEAT 10
#define UDP_PORT 12345

enum PacketType{ROOM=0,GET_ROOM=1,DELETE_ROOM=2};

struct DatagramPacket{
  quint8 type;
  qint32 id;
  QString name;
};

#endif
