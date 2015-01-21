#include "server_connection.h"
#include <iostream>
ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    socket->abort();
    socket->connectToHost(host, PORT_NUMBER);
    if(socket->waitForConnected(3000))
        qDebug() << "connected";
    //getSocketState(socket);
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
//        case PASS:      {emit gotPass(*pack.getData()->strData); break;}
        case MEMBER:    {makeMember(pack.getData()->rawData); break;}
//        case RAW: {emit gotRawData(pack.getData()->rawData, pack.getHeader()->length); break;}
        case INVALID_PASS: {emit gotInvalidPass(); break;}
//        case ADRESS: {emit gotAdress(*pack.getData()->strData); break;}
        case REMOVE: {emitRemoveMember(pack.getData()->rawData); break;}
    default: throw pack.getHeader()->type;
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

void ServerConnection::emitRemoveMember(char* block){
    QDataStream in(block);
    qint32 address;
    QHostAddress addr;
    in >> address;
    addr = QHostAddress(address);
    emit(deleteMember(addr));
}

void ServerConnection::throwSocketError(QAbstractSocket::SocketError err)
{
    qDebug() << err;
}

void ServerConnection::connected()
{
    qDebug() << "connected";
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
}

void ServerConnection::getSocketState (QTcpSocket* socket)
{
   switch (socket->state ())
   {
      case QAbstractSocket::UnconnectedState:
         qDebug() << (tr("unconnected state"));
         break;
      case QAbstractSocket::HostLookupState:
         qDebug() << (tr("host lookup state"));
         break;
      case QAbstractSocket::ConnectingState:
         qDebug() << (tr("connecting state"));
         break;
      case QAbstractSocket::ConnectedState:
         qDebug() << (tr("connected state"));
         break;
      case QAbstractSocket::BoundState:
         qDebug() << (tr("bound state"));
         break;
      case QAbstractSocket::ClosingState:
         qDebug() << (tr("Closing state"));
         break;
      case QAbstractSocket::ListeningState:
         qDebug() << (tr("Listening  state"));
         break;
      default:
         qDebug() << (tr("Unknown state"));
         break;
   }
}
