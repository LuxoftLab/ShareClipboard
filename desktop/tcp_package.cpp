#include "tcp_package.h"

void ServerConnectionHandlerText::decode(QDataStream &in)
{
    in >> size;
    text = new char[size];
    in >> text;
    //emit gotText(QString::fromUtf8(text).toUtf8());
    emit gotData(QByteArray::fromRawData(text, size), "text/plain");
}

void ServerConnectionHandlerMember::decode(QDataStream &in)
{
    in >> size;
    login = new char[size];
    in >> login;
    in >> address;
    emit addMember(QString::fromUtf8(login), QHostAddress(address));
}

ServerConnectionHandler *ServerConnectionFactory::getHandler(pckg_t packt)
{
    if(packt < 0)
        qDebug() << "No data delivered";
    switch(packt){
        case MEMBER:
            return new ServerConnectionHandlerMember();
       case TEXT:
            return new ServerConnectionHandlerText();
       case REMOVE:
            return new ServerConnectionHandlerRemoveMember();
       case IMAGE:
            return new ServerConnectionHandlerImage();
       default: throw packt;
    }
}

void ServerConnectionHandlerRemoveMember::decode(QDataStream &in)
{
    in >> address;
    emit(deleteMember(QHostAddress(address)));
}


void ServerConnectionHandlerImage::decode(QDataStream &in)
{
    in >> size;
    //image = new char[size];
    QByteArray image;
    in >> image;
    //emit gotImage(QByteArray::fromRawData(image, size));
    //emit gotImage(image);
    emit gotData(image, "image/png");
}


//void ClientConnectionHandlerText::decode(QDataStream &in)
//{
//    in >> size;
//    text = new char[size];
//    in >> text;
//    //emit(onText(QString::fromUtf8(text), this));
//}


//void ClientConnectionHandlerVerifyPass::decode(QDataStream &in, ClientConnection * const c)
//{
//    in >> size;
//    password = new char[size];
//    in >> password;
//    in >> size2;
//    login = new char[size2];
//    in >> login;
//    emit(verifyPass(QString::fromUtf8(password, size), c));
//}
