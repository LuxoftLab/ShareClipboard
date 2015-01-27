#include "client_connection.h"

QByteArray ClientConnection::makeBinaryPack(pckg_t type, char* dat, int datsize){
    Data d;
    d.rawData = dat;
    TcpPackageHeader head = TcpPackageHeader(type, datsize);
    TcpPackage pack = TcpPackage(head, d);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

QByteArray ClientConnection::makeBinaryPack(pckg_t type, QString str){
    TcpPackageHeader head = TcpPackageHeader(type, str.size());
    Data dat;
    dat.strData = &str;
    TcpPackage pack = TcpPackage(head, dat);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

QByteArray ClientConnection::makeBinaryPack(pckg_t type, qint32 num)
{
    TcpPackageHeader head = TcpPackageHeader(type, 4);
    Data dat;
    sprintf(dat.rawData, "%d", num);
    TcpPackage pack = TcpPackage(head, dat);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << pack;
    return block;
}

void ClientConnection::makeMember(char* block)
{
    QDataStream in(block);
    int loginSize, pwdSize;
    in >> loginSize;
    char* login = new char[loginSize];
    in >> login;
    in >> pwdSize;
    char* pwd = new char[pwdSize];
    in >> pwd;

    //emit verifyPass(pwd, login, this);
}

void ClientConnection::makePass(char *block)
{
    int passSize, loginSize;
    QString pass;
    QString login;
    QDataStream in(block);
    in >> passSize;
    char* rawPass = new char[passSize];
    in >> loginSize;
    char* rawLogin = new char[loginSize];
    in >> rawLogin;
    pass = QString::fromUtf8(rawPass);
    login = QString::fromUtf8(rawLogin);
    emit(verifyPass(pass, this));
}

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    connect(socket, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
//    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
}

void ClientConnection::sendFail()
{
    socket->write(makeBinaryPack(INVALID_PASS, NULL, 0));
}

void ClientConnection::sendMember(QString login, QHostAddress addr)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << login.size() << login << addr.toString().size() << addr.toString();
    socket->write(makeBinaryPack(MEMBER, dat.data(), dat.size()));

}

void ClientConnection::removeMember(QHostAddress addr)
{
    socket->write(makeBinaryPack(REMOVE, addr.toIPv4Address()));
}

QString ClientConnection::getLogin() {
    return login;
}

void ClientConnection::onData(){
    QDataStream in(socket);
    TcpPackage pack;
    in >> pack;

    //make something sane here instead switch
    switch(pack.getHeader()->type){
        //case TEXT:      {emit gotText(*pack.getData()->strData); break;}
        case PASS:      {makePass(pack.getData()->rawData); break;}
        //case RAW: {emit gotRawData(pack.getData()->rawData, pack.getHeader()->length); break;}
        //case ADRESS: {emit gotAdress(*pack.getData()->strData); break;}
        //case REMOVE: {emit deleteMember(makeHostAdress(pack.getData()->rawData)); break;}
    default: throw pack.getHeader()->type;
    }
}

void ClientConnection::emitDeleteMember()
{

}

QHostAddress ClientConnection::makeHostAdress(char* block){
    QHostAddress* address = new QHostAddress;
    return *address;
}


void ClientConnection::sendText(QString text)
{
    //nothing to do here?
}

