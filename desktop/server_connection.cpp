#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    //transferFinished = true;
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(serverFell()));

    socket->connectToHost(host, PORT_NUMBER);
    if(!socket->waitForConnected(3000))
        qDebug() << socket->error();
}

void ServerConnection::sendPassLoginPriority(QString password,
                                             QString login,
                                             floating_server_priorities priority)
{
    PassPackage(password, priority).write(socket);
}

//void ServerConnection::onData()
//{
//    QDataStream in(socket);
//    if(transferFinished)
//    {
//        in >> currenFiletSize;
//        transferFinished = false;
//        file.clear();
//    }
//    downloadMore(file, socket);
//}

void ServerConnection::dispatch(QDataStream &in)
{
    qint32 packt;
    in >> packt;
    hand = TcpPackageFactory().getPackage((pckg_t)packt);
    connect(hand, SIGNAL(gotText(QString)), this, SIGNAL(gotText(QString)));
    connect(hand, SIGNAL(gotImage(QByteArray)),
            this, SIGNAL(gotImage(QByteArray)));
    connect(hand, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(hand, SIGNAL(addMember(QString,floating_server_priorities,QHostAddress)),
            this, SIGNAL(addMember(QString,floating_server_priorities,QHostAddress)));
    connect(hand, SIGNAL(deleteMember(QHostAddress)),
            this, SIGNAL(deleteMember(QHostAddress)));
    hand->read(in);
}

void ServerConnection::sendData(QByteArray data, pckg_t type)
{
    DataPackage(data, type).write(socket);
}
