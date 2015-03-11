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
    hand->decode(in);
//    if(packt == 0)
//        qDebug() << "No data delivered";
//    switch(packt){
//        case MEMBER:
//            makeMember(in);
//            break;
//        case REMOVE:
//            removeMember(in);
//            break;
//        case INVALID_PASS:
//            emit(gotInvalidPass());
//            break;
//        case TEXT:
//            makeText(in);
//            break;
//        case IMAGE:
//            makeImage(in);
//            break;
//        default: throw packt;
//    }
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

void ServerConnection::sendImage(QByteArray image)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << IMAGE <<  (qint32)image.size() << image.constData();

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

//---------------------- case handlers ------------------

void ServerConnection::makeMember(QDataStream & in)
{
    int size;
    qint32 address;
    in >> size;
    char * login = new char[size];
    in >> login;    
    in >> address;
    emit addMember(QString::fromUtf8(login), QHostAddress(address));
}

void ServerConnection::removeMember(QDataStream & in)
{
    int addr;
    in >> addr;
    emit(deleteMember(QHostAddress(addr)));
}

void ServerConnection::makeText(QDataStream & in)
{
    int size;
    in >> size;
    char * text = new char[size];
    in >> text;
    emit gotText(QString::fromUtf8(text).toUtf8());
}

void ServerConnection::makeImage(QDataStream & in)
{
    int size;
    in >> size;
    char * image = new char[size];
    in >> image;
    //emit gotImage(QByteArray, "text/plain");
}
