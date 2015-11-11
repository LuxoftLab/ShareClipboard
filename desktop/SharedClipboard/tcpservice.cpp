#include "tcpservice.h"

TcpService::TcpService(QObject *parent) : QObject(parent)
{
    ownSocket = new QTcpSocket(this);
    QString key = "aaaaa"; // ###
    encoder = QSharedPointer<Encoder>(new Encoder(key));
}

void TcpService::createServer()
{
    ownServer = new QTcpServer(this);
    if(!ownServer->listen(QHostAddress::Any, TCP_PORT)){
        throw 2; //###
    }
    connect(ownServer, SIGNAL(newConnection()),
            this, SLOT(registerConnection()));
}

void TcpService::connectSocket(QHostAddress dest)
{
    ownSocket->connectToHost(dest, TCP_PORT);
    ownSocket->waitForConnected(WAIT_FOR_CONN_TIME_MSEC);

    if(ownSocket->state() == QAbstractSocket::UnconnectedState)
        throw 3; //###

    connect(ownSocket, &QTcpSocket::readyRead,
            this, &TcpService::read);
}

void TcpService::authenticate(QString login)
{
    QByteArray temp = login.toUtf8(); //WTF??
    this->write(AUTH, temp, ownSocket);
}

void TcpService::sendData(TcpPackage type, QByteArray & data)
{
//    for(auto memberit = roomMembers.begin(); memberit != roomMembers.end(); ++memberit){
//        write(type, data, *memberit);
//    }
}

void TcpService::addRoomMembers(QList<QHostAddress>)
{
    //connect to each qhostaddress and add sockets to list
}

void TcpService::write(TcpPackage type, QByteArray & data, QTcpSocket * dest)
{
    QByteArray temp;
    QDataStream out(&temp, QIODevice::WriteOnly);
    qint32 size_all = 0;

    out << (qint32)type << (qint32)data.size();
    size_all += sizeof(qint32) + sizeof(qint32);
    out.writeRawData(data.constData(), data.size());
    size_all += data.size();

    if(dest->write(encoder->encode(temp)) < size_all){
        throw 5;
        //###
    }

}

void TcpService::registerConnection()
{
    QTcpSocket * nextConnection = ownServer->nextPendingConnection();
//    if(!roomMembers.contains(nextConnection)){
//        roomMembers.append(nextConnection);
//        connect(nextConnection, &QTcpSocket::readyRead,
//                this, &TcpService::read);
//        connect(nextConnection, &QTcpSocket::disconnected,
//                [=](){ if(!roomMembers.removeOne(qobject_cast<QTcpSocket*>(QObject::sender()))) throw 4; }); //###
//    }
}

//void TcpService::removeMember()
//{

//}

void TcpService::read()
{
    QByteArray temp;
    QDataStream in(&temp, QIODevice::ReadOnly);

    while(ownSocket->bytesAvailable() > 0){
        temp.append(ownSocket->readAll());
    }

    if(temp.size() > 0){
        temp = encoder->decode(temp);

        qint32 packageType;
        in >> packageType;
        qint32 size;
        in >> size;
        char * name = new char[size];
        in >> name;

        qDebug() << (TcpPackage)packageType << size << name;
        delete[] name;
    }
}
