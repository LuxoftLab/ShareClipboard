#include "connection.h"

Connection::Connection(QTcpSocket * socket)
{
    this->socket = socket;
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onData(qint64)));
}

QHostAddress Connection::getIpv4()
{
    return socket->peerAddress();
}

QByteArray Connection::makeBinaryPack(pckg_t type, char* dat, int datsize){
    Data d;
    d.rawData = dat;
    TcpPackageHeader head = TcpPackageHeader(type, datsize);
    TcpPackage pack = TcpPackage(head, d);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

void Connection::onData(qint64){
    QDataStream in(socket);
    TcpPackage pack;
    in >> pack;
    //emit(gotData(&pack));

    //make something sane here instead switch
    switch(pack.getHeader()->type){
        case TEXT:      {emit gotText(*pack.getData()->strData); break;}
        case PASS:      {emit gotPass(*pack.getData()->strData); break;}
        case MEMBER:    {emit gotMember(makeMember(pack.getData()->rawData)); break;}
        case RAW: {emit gotRawData(pack.getData()->rawData, pack.getHeader()->length); break;}
        case INVALID_PASS: {emit gotInvalidPass(); break;}
        case ADRESS: {emit gotAdress(*pack.getData()->strData); break;}
        case REMOVE: {emit gotRemoveRequest(*pack.getData()->strData); break;}
    default: throw pack.getHeader()->type;
    }
}

QPair<QString, QString> Connection::makeMember(char* block){
    QDataStream in(block);
    int loginSize, pwdSize;
    in >> loginSize;
    char* login = new char[loginSize];
    in >> login;
    in >> pwdSize;
    char* pwd = new char[pwdSize];
    in >> pwd;
    QString loginStr = QString::fromUtf8(login);
    QString pwdStr = QString::fromUtf8(pwd);
    QPair<QString, QString> member;
    member.first = loginStr;
    member.second = pwdStr;
    return member;
}
