#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    transferFinished = true;
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(serverFell()));

    try
    {
        socket->connectToHost(host, PORT_NUMBER);
    }
    catch(QAbstractSocket::SocketError)
    {
        throw;
    }

    if(!socket->waitForConnected(3000))
        qDebug() << socket->error();
}

void ServerConnection::sendPassLoginPriority(QString password, QString login, floating_server_priorities priority)
{
   QByteArray dat;
   QDataStream out(&dat, QIODevice::WriteOnly);
   out << qint32(0) << PASS
       << (qint32)priority
       << (qint32)password.toUtf8().size()
       << password.toUtf8().data();
   out.device()->seek(0);
   out << (qint32)(dat.size() - sizeof(qint32));

   if(socket->write(dat) == 0)
   {
       qDebug() << "No data written";
   }
}

void ServerConnection::onData()
{
    QDataStream in(socket);
    if(transferFinished)
    {
        in >> currenFiletSize;
        transferFinished = false;
        file.clear();
    }
    downloadMore(file, socket);
}

void ServerConnection::dispatch(QDataStream &in)
{
    qint32 packt;
    in >> packt;
    hand = (new ServerConnectionFactory())->getHandler((pckg_t)packt);
    connect(hand, SIGNAL(gotText(QString)), this, SIGNAL(gotText(QString)));
    connect(hand, SIGNAL(gotImage(QByteArray)),
            this, SIGNAL(gotImage(QByteArray)));
    connect(hand, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(hand, SIGNAL(addMember(QString,floating_server_priorities,QHostAddress)),
            this, SIGNAL(addMember(QString,floating_server_priorities,QHostAddress)));
    connect(hand, SIGNAL(deleteMember(QHostAddress)),
            this, SIGNAL(deleteMember(QHostAddress)));
    hand->decode(in);
}

void ServerConnection::sendData(QByteArray data, pckg_t type)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << type << (qint32)data.size();
    out.writeRawData(data.constData(), data.size());
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    out.device()->seek(4+4+4+data.size()+1);

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}
