#include "udp_service.h"

UDPService::UDPService() : QObject(0),
    udp_socket(0),
    last_packadge_id(-1)
{
}

bool UDPService::initListener(){

    QTime now = QTime::currentTime();
    qsrand(now.msec());

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            localhost_ip=address;

    foreach(QNetworkInterface iface, QNetworkInterface::allInterfaces())

        if (iface.isValid() && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) //!iface.flags().testFlag(QNetworkInterface::IsUp) &&

            foreach(QNetworkAddressEntry address_ent, iface.addressEntries())
                if(!address_ent.broadcast().isNull())
                    broadcasts<<address_ent.broadcast();


    udp_socket=new QUdpSocket();
    if (!udp_socket->bind(UDP_PORT,QUdpSocket::ShareAddress))
        return 1;

    connect(udp_socket,&QUdpSocket::readyRead,this,&UDPService::listener);

    return 0;
}

void UDPService::getRooms(){   
    DatagramPacket packet;

    packet.type=GET_ROOM;
    packet.id=qrand();

    sendBroadcastPackadge(packet);
}

void UDPService::notifyAboutRoom(QString name){
    DatagramPacket packet;

    packet.type=ROOM;
    packet.id=qrand();
    packet.name=name;

    sendBroadcastPackadge(packet);
}

void UDPService::notifyAboutRoomDeleting(){
    DatagramPacket packet;

    packet.type=DELETE_ROOM;
    packet.id=qrand();

    sendBroadcastPackadge(packet);
}

void UDPService::sendBroadcastPackadge(const DatagramPacket &packet){

    foreach(QHostAddress addr, broadcasts)
        sendPackage(addr,packet);
}

void UDPService::sendPackage(const QHostAddress &peer, const DatagramPacket &packet){
    QBuffer data_buffer;
    data_buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&data_buffer);

    stream<<packet.type;
    stream<<packet.id;

    if(packet.type==ROOM)
        stream<<packet.name;

    QUdpSocket socket;

    for(int i=0;i<REPEAT;++i)
        socket.writeDatagram(data_buffer.data(),peer,UDP_PORT);
}

void UDPService::sendRoom(QString name){
    DatagramPacket packet;

    packet.type=ROOM;
    packet.id=qrand();
    packet.name=name;
    foreach(QHostAddress addr, senders)
        sendPackage(addr,packet);

    senders.clear();
}

void UDPService::listener(){

    while (udp_socket->hasPendingDatagrams()){
        QByteArray data;
        data.resize(udp_socket->pendingDatagramSize());
        QHostAddress sender_adr;
        udp_socket->readDatagram(data.data(),data.size(),&sender_adr);

        DatagramPacket packet;
        QDataStream stream(&data,QIODevice::ReadOnly);

        stream>>packet.type;
        stream>>packet.id;
        if(packet.type==ROOM) //||packet.type==DELETE_ROOM?
            stream>>packet.name;

        if(packet.id==last_packadge_id)
            continue;
        last_packadge_id=packet.id;

        switch(packet.type){

            case ROOM:
                emit roomReceived(packet.name,sender_adr);
            break;

            case GET_ROOM:

                if(sender_adr!=localhost_ip){
                    emit roomRequested();
                    senders.push_back(sender_adr);
                }

            break;

            case DELETE_ROOM:
                emit roomDeleted(sender_adr);
                //sendBroadcastPackadge(DELETE_ROOM);
            break;
        }
    }
}
