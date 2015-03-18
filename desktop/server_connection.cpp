 #include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    hand = (new ServerConnectionFactory())->getHandler((pckg_t)0);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
    socket->abort();
    socket->connectToHost(host, PORT_NUMBER);
    if(!socket->waitForConnected(3000))
        qDebug() << socket->error();
}

int ServerConnection::sendPassAndLogin(QString password, QString login)
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

void ServerConnection::connected()
{
    qDebug() << "connected";
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
    connect(hand, SIGNAL(addMember(QString,QHostAddress)),
            this, SIGNAL(addMember(QString,QHostAddress)));
    connect(hand, SIGNAL(deleteMember(QHostAddress)),
            this, SIGNAL(deleteMember(QHostAddress)));
    hand->decode(in);
}
void ServerConnection::sendText(QString text)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << TEXT << text.toUtf8().size() << text.toUtf8();

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

void ServerConnection::sendImage(QImage image)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "png");

    out << IMAGE << ba.size() << ba;
    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}
