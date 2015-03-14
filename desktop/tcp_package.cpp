#include "tcp_package.h"

void ServerConnectionHandlerText::decode(QDataStream &in)
{
    in >> size;
    text = new char[size];
    in >> text;
    emit gotText(QString::fromUtf8(text).toUtf8());
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
       default: throw packt;
    }
}

void ServerConnectionHandlerRemoveMember::decode(QDataStream &in)
{
    in >> address;
    emit(deleteMember(QHostAddress(address)));
}
