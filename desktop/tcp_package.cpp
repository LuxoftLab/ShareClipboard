#include "tcp_package.h"

TcpPackage *TcpPackageFactory::getPackage(pckg_t type)
{
    switch(type){
        case MEMBER:
            return new MemberPackage();
       case TEXT:
            return new DataPackage();
       case REMOVE:
            return new RemoveMemberPackage();
       case IMAGE:
            return new ImagePackage();
       case FILENOTIF:
            return new DataPackage();
       case PASS:
            return new PassPackage();
       default: throw type;
    }
}

DataPackage::DataPackage(QByteArray& data, pckg_t type)
{
    this->data = data;
    this->type = type;
}

DataPackage::DataPackage()
{

}

void DataPackage::read(QDataStream & in)
{
    in >> size;
    text = new char[size];
    in.readRawData(text, size);

    emit gotText(QByteArray::fromRawData(text, size));
    emit gotData(QByteArray::fromRawData(text, size), "text/plain");
}

void DataPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << type << (qint32)data.size();
    out.writeRawData(data.constData(), data.size());
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    out.device()->seek(4+4+4+data.size()+1);

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}

PassPackage::PassPackage(QString pass, floating_server_priorities prior)
{
    this->password = pass;
    this->priority = prior;
}

PassPackage::PassPackage()
{

}

void PassPackage::read(QDataStream & in)
{
    qint32 priority;
    qint32 pwdsz;
    in >> priority;
    in >> pwdsz;
    char* pwd = new char[pwdsz];
    in >> pwd;
    emit gotPass(QString::fromUtf8(pwd, pwdsz), (floating_server_priorities)priority);
}

void PassPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << qint32(0) << PASS
        << (qint32)priority
        << (qint32)password.toUtf8().size()
        << password.toUtf8().data();
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    socket->write(dat);
}

MemberPackage::MemberPackage()
{

}

MemberPackage::MemberPackage(QString login, QHostAddress addr, floating_server_priorities priority)
{
    this->login = login;
    this->addr = addr;
    this->prior = priority;
}

void MemberPackage::read(QDataStream & in)
{
    qint32 priority;
    in >> priority;
    in >> size;
    char* login = new char[size];
    in >> login;
    in >> address;
    emit addMember(QString::fromUtf8(login),
                   (floating_server_priorities)priority,
                   QHostAddress(address));
    delete login;
}

void MemberPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << qint32(0) << MEMBER
        << prior
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

ImagePackage::ImagePackage()
{

}

void ImagePackage::write(QTcpSocket *)
{

}

void ImagePackage::read(QDataStream & in)
{
    in >> size;
    image = new char[size];
    in.readRawData(image,size);

    emit gotData(QByteArray(image, size), "image/png");
    emit gotImage(QByteArray(image, size));
}


RemoveMemberPackage::RemoveMemberPackage()
{

}

RemoveMemberPackage::RemoveMemberPackage(QHostAddress toRemove)
{
    this->removeAddr = toRemove;
}

void RemoveMemberPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << (qint32)(sizeof(qint32)) << REMOVE << removeAddr.toIPv4Address();

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

void RemoveMemberPackage::read(QDataStream &)
{

}


void FailPackage::read(QDataStream &)
{

}

void FailPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << INVALID_PASS;

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

