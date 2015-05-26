 #include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));

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

void ServerConnection::sendPassAndLogin(QString password, QString login)
{
   QByteArray dat;
   QDataStream out(&dat, QIODevice::WriteOnly);
   out << PASS << password.toUtf8().size() << password.toUtf8().data()
       << login.toUtf8().size() << login.toUtf8().data();

   if(socket->write(dat) == 0)
   {
       qDebug() << "No data written";
   }
}

void ServerConnection::onData()
{
    QDataStream in(socket);
    qint32 packt;
    in >> packt;
    hand = (new ServerConnectionFactory())->getHandler((pckg_t)packt);
    connect(hand, SIGNAL(gotText(QString)), this, SIGNAL(gotText(QString)));
    connect(hand, SIGNAL(gotImage(QByteArray)),
            this, SIGNAL(gotImage(QByteArray)));
    connect(hand, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(hand, SIGNAL(addMember(QString,QHostAddress)),
            this, SIGNAL(addMember(QString,QHostAddress)));
    connect(hand, SIGNAL(deleteMember(QHostAddress)),
            this, SIGNAL(deleteMember(QHostAddress)));
    if(packt == IMAGE)
    {
        QByteArray temp;

        while(socket->bytesAvailable() > 0)
        {
            temp.append(socket->readAll());
        }
        QDataStream out(&temp, QIODevice::ReadOnly);
        hand->decode(out);
    }
    else hand->decode(in);
}

void ServerConnection::sendData(QByteArray data, pckg_t type)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << type << (qint32)data.size() << data;

    QImage image2 = QImage::fromData(data);
    image2.save("/home/asalle/2.png");
    qDebug() << dat.size();
    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}
