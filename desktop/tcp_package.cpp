#include "tcp_package.h"



void ServerConnectionHandlerText::decode(QDataStream &in)
{
    int size;
    in >> size;
    char * text = new char[size];
    in >> text;
    emit gotText(QString::fromUtf8(text).toUtf8());
}


void ServerConnectionHandlerMember::decode(QDataStream &in)
{
    int size;
    qint32 address;
    in >> size;
    char * login = new char[size];
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
       default: throw packt;
    }
}


void ServerConnectionHandler::decode(QDataStream &in)
{

}
