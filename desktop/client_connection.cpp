#include "client_connection.h"

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    this->login = "login";
    transferFinished = true;
    connect(socket, SIGNAL(disconnected()), this, SLOT(emitDeleteMember()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
}

void ClientConnection::sendFail()
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << INVALID_PASS;

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

void ClientConnection::sendMember(QString login, floating_server_priorities priority, QHostAddress addr)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << qint32(0) << MEMBER
        << priority
        << login.toUtf8().size()
        << login.toUtf8().data()
        << addr.toIPv4Address();
    out.device()->seek(0);
    out << qint32(dat.size() - sizeof(qint32));

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

void ClientConnection::removeMember(QHostAddress addr)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << REMOVE << addr.toIPv4Address();

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

QString ClientConnection::getLogin() {
    return login;
}

void ClientConnection::onData(){
    QDataStream in(socket);
    QDataStream infile(&file, QIODevice::ReadOnly);
    if(transferFinished)
    {
        file.clear();
        transferFinished = false;
        in >> currenFiletSize;
        currenFiletSize -= sizeof(packt);
        in >> packt;
    }
    downloadMore(file, socket);
}

QHostAddress ClientConnection::makeHostAdress(char* block){
    QHostAddress* address = new QHostAddress;
    return *address;
}

void ClientConnection::sendData(QByteArray arr, pckg_t type)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << type << (qint32)arr.size() << arr;
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    out.device()->seek(4+4+4+arr.size());

    QImage image2 = QImage::fromData(arr);
    image2.save("/home/asalle/4.png");
    int written = 0;
    if((written = socket->write(dat)) < dat.size())
    {
        qDebug() << "additional image transfer needed";
    }
    qDebug() << written;
}

//----------------- case handlers --------------------

void ClientConnection::makePass(QDataStream& in)
{
    qint32 priority;
    qint32 pwdsz;
    in >> priority;
    in >> pwdsz;
    char* pwd = new char[pwdsz];
    in >> pwd;
    qDebug() << pwd << pwdsz;
    emit(verifyPass(QString::fromUtf8(pwd, pwdsz), (floating_server_priorities)priority, this));
}

void ClientConnection::downloadMore(QByteArray& whole, QTcpSocket * inSocket)
{
    QByteArray file;
    QDataStream in(&whole, QIODevice::ReadOnly);
    while(inSocket->bytesAvailable() > 0)
    {
        file = inSocket->readAll();
        whole.append(file);
        assert(file.size() > 0);
    }
    if(whole.size() >= currenFiletSize)
    {
        transferFinished = true;
        dispatch(in);
    }
}

void ClientConnection::dispatch(QDataStream& infile)
{
    switch(packt){
        case PASS:
            makePass(infile);
            break;
        case TEXT:
            emitText(infile);
            break;
        case IMAGE:
            emitImage(infile);
            break;
        default: throw packt;
    }
}

void ClientConnection::emitText(QDataStream& in)
{
    qint32 size;
    in >> size;
    char* text = new char[size];
    in >> text;
    emit(onText(QString::fromUtf8(text), this));
}

void ClientConnection::emitImage(QDataStream& in)
{
    qint32 size;
    in >> size;
    //char* image = new char[size];
    QByteArray image;
    in >> image;
    QImage image2 = QImage::fromData(image);
    image2.save("/home/asalle/3.png");

    //emit onImage(QByteArray::fromRawData(image, size), this);
    emit onImage(image, this);
}
