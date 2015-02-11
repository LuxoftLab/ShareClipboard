#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
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
    if(packt == 0)
        qDebug() << "No data delivered";
    switch(packt){
        case MEMBER:
            makeMember(in);
            break;
        default: throw packt;
    }
}
void ServerConnection::sendText(QString text)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << text.size() << text;

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
