#include "server_connection.h"
<<<<<<< HEAD

=======
#include <iostream>
>>>>>>> origin/leonid
ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    socket->abort();
    socket->connectToHost(host, PORT_NUMBER);
<<<<<<< HEAD
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

=======

    if(!socket->waitForConnected(3000))
        qDebug() << socket->error();
//    //getSocketState(socket);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
              SLOT(throwSocketError(QAbstractSocket::SocketError)));
    //#todo error handlers
    //connect(socket, SIGNAL(readyRead()), this, SLOT(connectToServer()));
    //  connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onData(qint64)));
}


int ServerConnection::sendPassAndLogin(QString password, QString login){
   QByteArray dat;
   QDataStream out(&dat, QIODevice::WriteOnly);
   out << login.toUtf8().size() << login.toUtf8() << password.toUtf8().size() << password.toUtf8();
   if(socket->write(makeBinaryPack(PASS, dat.data(), dat.size())) == 0){
       qDebug() << "No data written";
       std::cout << "No data written";
   }
}

/*void ServerConnection::deleteMe(QHostAddress address){
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << address.toIPv4Address();
    socket->write(makeBinaryPack(REMOVE, dat.data(), dat.size()))   ;
}*/

void ServerConnection::onData()
{
    QDataStream in(socket);
    TcpPackage pack;
    in >> pack;

    //make something sane here instead switch
    switch(pack.getHeader()->type){
//        case TEXT:      {emit gotText(*pack.getData()->strData); break;}
        case PASS:      {emit gotPass(*pack.getData()->strData); break;}
        case MEMBER:    {makeMember(pack.getData()->rawData); break;}
//        case RAW: {emit gotRawData(pack.getData()->rawData, pack.getHeader()->length); break;}
        case INVALID_PASS: {emit gotInvalidPass(); break;}
//        case ADRESS: {emit gotAdress(*pack.getData()->strData); break;}
        case REMOVE: {emitRemoveMember(pack.getData()->rawData); break;}
    default: throw pack.getHeader()->type;
    }
}

>>>>>>> origin/leonid
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

<<<<<<< HEAD
void ServerConnection::emitRemoveMember(char* block)
{
=======
void ServerConnection::emitRemoveMember(char* block){
>>>>>>> origin/leonid
    QDataStream in(block);
    qint32 address;
    QHostAddress addr;
    in >> address;
    addr = QHostAddress(address);
    emit(deleteMember(addr));
}

<<<<<<< HEAD
=======
void ServerConnection::throwSocketError(QAbstractSocket::SocketError err)
{
    qDebug() << err;
}

>>>>>>> origin/leonid
void ServerConnection::connected()
{
    qDebug() << "connected";
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
}

<<<<<<< HEAD
void ServerConnection::onData()
{

=======
void ServerConnection::sendText(QString text)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << text.size() << text;
    socket->write(makeBinaryPack(TEXT, dat.data(), dat.size()));
>>>>>>> origin/leonid
}
