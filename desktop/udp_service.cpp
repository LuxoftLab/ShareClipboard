#include "udp_service.h"

UDPService::UDPService() : QObject(0),
    udp_socket(0),
    last_packadge_id(-1)
{
}

bool UDPService::initListener()
{
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
    sendBroadcastPackadge(GET_ROOM,"0");
}

void UDPService::sendBroadcastPackadge(int type, QString room_name){
    DatagramPacket packet;

    packet.type=type;
    packet.id=qrand();
    packet.name=room_name;

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

void UDPService::notifyAboutRoom(QString name){
    sendBroadcastPackadge(ROOM,name);
}

void UDPService::listener(){

    while (udp_socket->hasPendingDatagrams()){
        QByteArray data;
        data.resize(udp_socket->pendingDatagramSize());
        QHostAddress sender_adr;
        udp_socket->readDatagram(data.data(),data.size(),&sender_adr);

        DatagramPacket p;
        QDataStream stream(&data,QIODevice::ReadOnly);

        stream>>p.type;
        stream>>p.id;
        if(packet.type==ROOM||packet.type==DELETE_ROOM)
            stream>>p.name;

        if(p.id==last_packadge_id)
            continue;
        last_packadge_id=p.id;

        switch(p.type){

            case ROOM:
            //Отображение списка комнат
            break;

            case GET_ROOM:

                if(sender_adr!=localhost_ip){
                    //emit onGetRoom;
                    senders.push_back(sender_adr);
                }

            break;
        }
    }
}
