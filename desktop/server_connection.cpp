#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
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

void ServerConnection::makeMember(char *block)
{
    QString login;
    QHostAddress addr;
    QDataStream in(block);
    int loginSize, addressRaw;
    char* loginRaw;
    in >> loginSize;
    loginRaw = new char[loginSize];
    in >> addressRaw;
    addr = QHostAddress(addressRaw);
    emit addMember(login, addr);
}

void ServerConnection::emitRemoveMember(char* block)
{
    QDataStream in(block);
    qint32 address;
    QHostAddress addr;
    in >> address;
    addr = QHostAddress(address);
    emit(deleteMember(addr));
}

void ServerConnection::connected()
{
    qDebug() << "connected";
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
}

void ServerConnection::onData()
{

}
