#include "tcp_package.h"

void TextPackage::decode(QDataStream &in)
{
    in >> size;
    text = new char[size];
    in.readRawData(text, size);

    emit gotText(QByteArray::fromRawData(text, size));
    emit gotData(QByteArray::fromRawData(text, size), "text/plain");
}


TcpPackage *TcpPackageFactory::getPackage(pckg_t type)
{
    switch(type){
        case MEMBER:
            return new MemberPackage();
       case TEXT:
            return new TextPackage();
       case REMOVE:
            return new RemoveMemberPackage();
       case IMAGE:
            return new ImagePackage();
       case PASS:
            return new PassPackage();
       default: throw type;
    }
}


void MemberPackage::decode(QDataStream &in)
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

void RemoveMemberPackage::decode(QDataStream &in)
{
    in >> address;
    emit(deleteMember(QHostAddress(address)));
}


void ImagePackage::decode(QDataStream &in)
{
    in >> size;
    image = new char[size];
    in.readRawData(image,size);

    emit gotData(QByteArray(image, size), "image/png");
    emit gotImage(QByteArray(image, size));
}


void PassPackage::decode(QDataStream &in)
{
    qint32 priority;
    qint32 pwdsz;
    in >> priority;
    in >> pwdsz;
    char* pwd = new char[pwdsz];
    in >> pwd;
    emit gotPass(QString::fromUtf8(pwd, pwdsz), (floating_server_priorities)priority);
}
