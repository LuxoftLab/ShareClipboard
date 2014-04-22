#include "listener.h"

Listener::Listener(QObject *parent) :
    QObject(parent),
    m_udp_socket(0)
{
    connect(this,&Listener::peerFound,this,&Listener::onPeerFound);
    connect(this,&Listener::helloReceived,this,&Listener::onHelloReceived);
}

void Listener::listen()
{
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces()) {
        if (!iface.isValid() &&
                !iface.flags().testFlag(QNetworkInterface::IsUp) &&
                iface.flags().testFlag(QNetworkInterface::IsLoopBack))
            continue;
        foreach (QNetworkAddressEntry addr_ent, iface.addressEntries()) {
            if(addr_ent.broadcast().isNull())
                continue;
            m_broadcasts<<addr_ent.broadcast();
        }
    }
    m_udp_socket=new QUdpSocket();
    if (!m_udp_socket->bind(UDP_PORT,QUdpSocket::ShareAddress))
        qFatal("Can't bind UDP socket.");
    connect(m_udp_socket,&QUdpSocket::readyRead,this,&Listener::processPendingDatagrams);
    peerLookupUDP();
}

void Listener::peerLookupUDP()
{
    QBuffer data_buffer;
    DatagramPacket packet;
    Q_ASSERT(data_buffer.open(QIODevice::WriteOnly));
    QDataStream stream(&data_buffer);

    packet.type=PacketType::Lookup;
    packet.content=QHostInfo::localHostName();
    stream<<packet;

    QUdpSocket socket;
    foreach (QHostAddress addr, m_broadcasts) {
        for(int i=0;i<REPEAT;i++)
            socket.writeDatagram(data_buffer.data(),addr,UDP_PORT);
    }
}

void Listener::sendHello(QHostAddress peer)
{
    DatagramPacket packet;
    packet.type=PacketType::Hello;
    packet.content=QHostInfo::localHostName();
    send(peer,packet);
}

void Listener::sendAreYouHere(QHostAddress peer)
{
    DatagramPacket packet;
    packet.type=PacketType::AreYouHere;
    packet.content=QHostInfo::localHostName();
    send(peer,packet);
}

void Listener::sendClipboard(QHostAddress peer, QString text)
{
    DatagramPacket packet;
    packet.type=PacketType::Clipboard;
    packet.content=text;
    send(peer,packet);
}

void Listener::processPendingDatagrams()
{
    while (m_udp_socket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(m_udp_socket->pendingDatagramSize());
        QHostAddress sender_adr;
        m_udp_socket->readDatagram(data.data(), data.size(),&sender_adr);
        DatagramPacket p;
        QDataStream stream(&data,QIODevice::ReadOnly);
        stream>>p;
        if(p.type==PacketType::Lookup && p.content==QHostInfo::localHostName())
        {
            Logger::instance()<<TimeStamp()<<"Self test - OK!\n";
            return;
        }
        switch(p.type)
        {
        case PacketType::Lookup:
            emit peerFound(sender_adr,p.content);
            break;
        case PacketType::Hello:
            emit helloReceived(sender_adr,p.content);
            break;
        case PacketType::Clipboard:
            emit clipboardContentArrived(p.content);
            break;
        case PacketType::AreYouHere:
            emit areYouHereReceived(sender_adr,p.content);
            break;
        default:
            break;
        }
    }
}

void Listener::onPeerFound(QHostAddress peer, QString nick)
{
    Logger::instance()<<TimeStamp()<<nick<<": lookup\n";
    sendHello(peer);
}

void Listener::onHelloReceived(QHostAddress peer, QString nick)
{
    Logger::instance()<<TimeStamp()<<nick<<": hello!\n";
    //sendHello(peer);
}

void Listener::onAreYouHereReceived(QHostAddress peer, QString nick)
{
    Logger::instance()<<TimeStamp()<<nick<<": are you here?\n";
    sendHello(peer);
}

void Listener::send(const QHostAddress &peer, const DatagramPacket &packet)
{
    QBuffer data_buffer;
    Q_ASSERT(data_buffer.open(QIODevice::WriteOnly));
    QDataStream stream(&data_buffer);
    stream<<packet;
    QUdpSocket socket;
    for(int i=0;i<REPEAT;i++)
        socket.writeDatagram(data_buffer.data(),peer,UDP_PORT);
}

