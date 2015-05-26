#include "client_connection.h"

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    this->login = "login";
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

void ClientConnection::sendMember(QString login, QHostAddress addr)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << MEMBER << login.toUtf8().size() << login.toUtf8().data() << addr.toIPv4Address();

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
    qint32 packt;
    QByteArray temp;
    QDataStream inimage(&temp, QIODevice::ReadOnly);
    in >> packt;
    if(packt == 0)
        qDebug() << "No data delivered";
    switch(packt){
        case PASS:
            makePass(in);
            break;
        case TEXT:
            emitText(in);
            break;
        case IMAGE:            
            while(socket->bytesAvailable() > 0)
                temp.append(socket->readAll());
            emitImage(inimage);
            break;
        default: throw packt;
    }
}

QHostAddress ClientConnection::makeHostAdress(char* block){
    QHostAddress* address = new QHostAddress;
    return *address;
}

//void ClientConnection::sendText(QString s)
//{
//    QByteArray dat;
//    QDataStream out(&dat, QIODevice::WriteOnly);
//    out << TEXT << s.toUtf8().size() << s.toUtf8();
//    if(socket->write(dat) == 0)
//    {
//        qDebug() << "No data written";
//    }
//}

//void ClientConnection::sendImage(QByteArray im)
//{
//    QByteArray dat;
//    QDataStream out(&dat, QIODevice::WriteOnly);

//    out << IMAGE << (qint32)im.size() << im;
//    if(socket->write(dat) == 0)
//    {
//        qDebug() << "No data written";
//    }
//}

void ClientConnection::sendData(QByteArray arr, pckg_t type)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << type << (qint32)arr.size() << arr;

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
    QByteArray temp;
    while(image.size() < size)
    {
        in >> temp;
        assert(temp.size() > 0);
        image.append(temp);
        qDebug() << image.size() << temp.size();
    }

    QImage image2 = QImage::fromData(image);
    image2.save("/home/asalle/3.png");

    //emit onImage(QByteArray::fromRawData(image, size), this);
    emit onImage(image, this);
}
