#include "client_room.h"


QString ClientRoom::getLogin() const
{
    return login;
}

void ClientRoom::setLogin(const QString &value)
{
    login = value;
}

QString ClientRoom::getPwd() const
{
    return pwd;
}

void ClientRoom::setPwd(const QString &value)
{
    pwd = value;
}
floating_server_priorities ClientRoom::device_type()
{
    return PC;
}

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

    connect(connection, SIGNAL(addMember(QString, floating_server_priorities, QHostAddress)),
            this, SLOT(addMember(QString, floating_server_priorities, QHostAddress)));
    connect(connection, SIGNAL(deleteMember(QHostAddress)),
            this, SLOT(deleteMember(QHostAddress)));
    connect(connection, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(connection, SIGNAL(serverFell()), this, SLOT(recoverServer()));
//    connect(connection, SIGNAL(gotText(QString)),//##
//            this, SIGNAL(gotText(QString)));
//    connect(connection, SIGNAL(gotImage(QByteArray)),
//            this, SIGNAL(gotImage(QByteArray)));
//    connect(connection, SIGNAL(setNotUpdated()),
//            this, SLOT(setNotUpdated()));

    connection->sendPassLoginPriority(pass, login, device_type());
}

void ClientRoom::addMember(QString login, floating_server_priorities prior, QHostAddress addr) {
    Member* newMember = new Member(login, addr, prior);
    members.insert(addr.toIPv4Address(), newMember);
    floating_server_candidates.append(newMember);
}

void ClientRoom::deleteMember(QHostAddress addr) {
    members.remove(addr.toIPv4Address());
}

//void ClientRoom::sendText(QString text)
//{
//    //connection->sendText(text);
//}

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
    QImage image2 = QImage::fromData(data);
    image2.save("/home/asalle/1.png");
    connection->sendData(data, p_type);
}

void ClientRoom::recoverServer()
{
    if(floating_server_candidates.size() > 0 && floating_server_candidates.first()->addr == host)
        emit newFloatingServer(host);
}


//void ClientRoom::sendImage(QImage image)
//{
//    connection->sendImage(image);
//    qDebug() << image.height() << image.width();
//}

//void ClientRoom::updateBuffer()
//{
//   // updated = true;
//}

//void ClientRoom::setNotUpdated()
//{
//    //updated = false;
//}

ClientRoom::~ClientRoom()
{
    for(QMap<qint32, Member*>::iterator it = members.begin(); it != members.end(); ++it)
        delete it.value();
    delete connection;
}

Member::Member(QString login, QHostAddress addr, floating_server_priorities prior)
{
    this->login = login;
    this->addr = addr;
    this->priority = prior;
}

