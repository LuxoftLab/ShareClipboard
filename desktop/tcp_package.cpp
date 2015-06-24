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
       case PASS:
            return new PassPackage();
       default: throw type;
    }
}

//void RemoveMemberPackage::decode(QDataStream &in)
//{
//    in >> address;
//    emit(deleteMember(QHostAddress(address)));
//}

//void ImagePackage::decode(QDataStream &in)
//{
//    in >> size;
//    image = new char[size];
//    in.readRawData(image,size);

//    emit gotData(QByteArray(image, size), "image/png");
//    emit gotImage(QByteArray(image, size));
//}

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


void MemberPackage::read(QDataStream & in)
{
    qint32 priority;
    in >> priority;
    in >> size;
    login = new char[size];
    in >> login;
    in >> address;
    emit addMember(QString::fromUtf8(login),
                   (floating_server_priorities)priority,
                   QHostAddress(address));
}

void MemberPackage::write(QTcpSocket *)
{

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


void RemoveMemberPackage::write(QTcpSocket *)
{

}

void RemoveMemberPackage::read(QDataStream &)
{

}
