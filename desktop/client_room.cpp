#include "client_room.h"

ClientRoom::ClientRoom(QString name, QHostAddress host) : Room(name, "")
{
    this->host = host;
}

void ClientRoom::connectToHost(QString login, QString pass)
{
    this->pass = pass;
    try{
    connection = new ServerConnection(host);
    }
    catch(QAbstractSocket::SocketError)
    {
        throw;
    }

    connect(connection, SIGNAL(addMember(QString, QHostAddress)),
            this, SLOT(addMember(QString, QHostAddress)));
    connect(connection, SIGNAL(deleteMember(QHostAddress)),
            this, SLOT(deleteMember(QHostAddress)));
    connect(connection, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(connection, SIGNAL(gotText(QString)),//##
            this, SIGNAL(gotText(QString)));
    connect(connection, SIGNAL(gotImage(QByteArray)),
            this, SIGNAL(gotImage(QByteArray)));
    connect(connection, SIGNAL(setNotUpdated()),
            this, SLOT(setNotUpdated()));

    connection->sendPassAndLogin(pass, login);
}

void ClientRoom::addMember(QString login, QHostAddress addr) {
    members.insert(addr.toIPv4Address(), new Member(login, addr));
}

void ClientRoom::deleteMember(QHostAddress addr) {
    members.remove(addr.toIPv4Address());
}

void ClientRoom::sendText(QString text)
{
    //connection->sendText(text);
}

void ClientRoom::sendData(QByteArray data, QString type)
{
    pckg_t p_type;
    if(type == "text/plain")
    {
         p_type = TEXT;
        //connection->sendText(QString::fromUtf8(data), updated, type);##
    }
    else if(type == "image/png")
        p_type = IMAGE;
//        connection->sendImage(data, updated);##
    else
    {
        qDebug() << "no such mime type available";
        return;
    }
    int sz = data.size(); //##
    connection->sendData(data, p_type);
}


void ClientRoom::sendImage(QImage image)
{
    connection->sendImage(image);
    qDebug() << image.height() << image.width();
}

void ClientRoom::updateBuffer()
{
   // updated = true;
}

void ClientRoom::setNotUpdated()
{
    //updated = false;
}

ClientRoom::~ClientRoom()
{
    for(QMap<qint32, Member*>::iterator it = members.begin(); it != members.end(); ++it)
        delete it.value();
    delete connection;
}

Member::Member(QString login, QHostAddress addr)
{
    this->login = login;
    this->addr = addr;
}

