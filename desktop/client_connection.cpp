#include "client_connection.h"

void ClientConnection::makePass(QDataStream& in)
{
    qint32 pwdsz;
    in >> pwdsz;
    char* pwd = new char[pwdsz];
    in >> pwd;
    qint32 lgnsz;
    in >> lgnsz;
    char* lgn = new char[lgnsz];
    in >> lgn;
    qDebug() << pwd << lgn;
    emit(verifyPass(QString::fromUtf8(pwd, pwdsz), this));
}

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    this->login = "login";
    connect(socket, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
}

void ClientConnection::sendFail()
{

}

void ClientConnection::sendMember(QString login, QHostAddress addr)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    int sz = login.toUtf8().size();
    out << MEMBER << login.toUtf8().size() << login.toUtf8().data() << addr.toIPv4Address();

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

void ClientConnection::removeMember(QHostAddress addr)
{

}

QString ClientConnection::getLogin() {
    return login;
}

void ClientConnection::onData(){
    QDataStream in(socket);
    qint32 packt;
    in >> packt;
    if(packt == 0)
        qDebug() << "No data delivered";
    switch(packt){
        case PASS:
            makePass(in);
            break;
        default: throw packt;
    }
}

void ClientConnection::sendText(QString text)
{
    //nothing to do here?
}

QHostAddress ClientConnection::makeHostAdress(char* block){
    QHostAddress* address = new QHostAddress;
    return *address;
}
