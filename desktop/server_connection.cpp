#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket->connectToHost(host, PORT_NUMBER);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
              //SLOT(throwSocketError(QAbstractSocket::SocketError)));
    //#todo error handlers
    //connect(socket, SIGNAL(readyRead()), this, SLOT(connectToServer()));
    //  connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onData(qint64)));
}


void ServerConnection::sendPassAndLogin(QString password, QString login){
   QByteArray dat;
   QDataStream out(&dat, QIODevice::WriteOnly);
   out << login.toUtf8().size() << login.toUtf8() << password.toUtf8().size() << password.toUtf8();
   socket->write(makeBinaryPack(PASS, dat.data(), dat.size()));
}

void ServerConnection::deleteMe(QHostAddress address){
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << address.toIPv4Address();
    socket->write(makeBinaryPack(REMOVE, dat.data(), dat.size()))   ;
}

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
//        case REMOVE: {emit gotRemoveRequest(*pack.getData()->strData); break;}
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
