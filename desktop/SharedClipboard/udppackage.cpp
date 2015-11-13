#include "udppackage.h"

IPackage::IPackage()
{

}



AlivePackage::AlivePackage(QString l, QString r, QList<QHostAddress> addrs) : login(l), room(r), ip_addresses(addrs)
{

}


void AlivePackage::write(QUdpSocket * socket)
{
    QByteArray broadcast;
    QDataStream out(&broadcast, QIODevice::WriteOnly);

    // login.size-login-roomName.size-roomName-hostAddress.count-hostAddress.asInt
    out << login.toUtf8().size();
    out.writeRawData(login.toUtf8().constData(), login.toUtf8().size());
    out << room.size();
    out.writeRawData(room.toUtf8().constData(),room.toUtf8().size());
    out << ip_addresses.size();
    for(auto address : ip_addresses){
        out << address.toIPv4Address();
    }

    socket->writeDatagram(broadcast, broadcast.size(), QHostAddress::Broadcast, UDP_PORT);
}

void AlivePackage::read(const QDataStream &)
{

}
