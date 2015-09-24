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
    connection = NULL;
}

void ClientRoom::connectToHost(QString login, QString pass)
{
    this->pass = pass;
    try{
        connection = new ServerConnection(host);
        connect(connection, SIGNAL(finished()),
                connection, SLOT(deleteLater()));
        connection->run();
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
    connect(connection, SIGNAL(gotFileNotification(QString,QHostAddress,QDateTime)),
            this, SIGNAL(gotFileNotification(QString,QHostAddress,QDateTime)));
    connect(connection, SIGNAL(serverFell()), this, SLOT(recoverServer()));

    connection->sendPassLoginPriority(pass, login, device_type());
}

void ClientRoom::addMember(QString login, floating_server_priorities prior, QHostAddress addr) {
    Member* newMember = new Member(login, addr, prior);
    members.insert(addr.toIPv4Address(), newMember);
    if(prior == PC)
        floating_server_candidates.push_front(newMember);
    else if(prior == MOBILE)
        floating_server_candidates.push_back(newMember);
}

void ClientRoom::deleteMember(QHostAddress addr) {
    members.remove(addr.toIPv4Address());
}

void ClientRoom::sendData(QByteArray data, QString type)
{
    pckg_t p_type;
    if(type == "text/plain"){
        p_type = TEXT;
        connection->sendData(data, p_type);
    }
    else if(type == "image/png"){
        p_type = IMAGE;
        connection->sendData(data, p_type);
    }
    else if (type == "text/uri-list"){
        p_type = FILENOTIF;
        QString text = QString::fromUtf8(data);
        text.remove(0, 7);
        text.remove(text.length()-2, 2);
        QFileInfo info(text);
        QDateTime timeStamp = info.lastModified();

        qDebug() << text << info.lastModified().date() << timeStamp.date();
        connection->sendFileNotification(data, timeStamp);
    }
    else{
        qDebug() << "no such mime type available";
        return;
    }

}

void ClientRoom::recoverServer()
{
    qDebug() << "recovering server";
    qDebug() << floating_server_candidates.first()->addr << connection->localAddress();
    if(floating_server_candidates.size() > 1 && floating_server_candidates.first()->addr == connection->localAddress())
        emit newFloatingServer(host);
}

void ClientRoom::fileNotification(QString name)
{

}

void ClientRoom::requestFile()
{
    const SharedFile * const requested = files.head();
    this->sendFileRequest(requested->name, requested->timeStamp);
}

void ClientRoom::addFile(QString n, QHostAddress s, QDateTime st)
{
    const SharedFile * const file = new SharedFile(n, s, st);
    this->files.push_back(file);
}

void ClientRoom::sendFileRequest(QString name, QDateTime timeStamp)
{
    connection->sendFileRequest(name, timeStamp);
}

ClientRoom::~ClientRoom()
{
    for(auto it = members.begin(); it != members.end(); ++it)
        delete it.value();
    for(auto it = files.begin(); it != files.end();  ++it)
        delete *it;
    delete connection;
}

Member::Member(QString login, QHostAddress addr, floating_server_priorities prior)
{
    this->login = login;
    this->addr = addr;
    this->priority = prior;
}


SharedFile::SharedFile(QString n, QHostAddress s, QDateTime st):
    name(n), sourceAddress(s), timeStamp(st)
{
}
