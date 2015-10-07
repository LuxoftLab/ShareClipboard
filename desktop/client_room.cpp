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

FloatServerPriority ClientRoom::device_type()
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
        //std::unique_ptr<ServerConnection> connection(new ServerConnection(host));
        connect(connection, SIGNAL(finished()),
                connection, SLOT(deleteLater()));
        connection->run();
    }
    catch(QAbstractSocket::SocketError)
    {
        throw;
    }

    connect(connection, SIGNAL(addMember(FloatServerPriority, QHostAddress)),
            this, SLOT(addMember(FloatServerPriority, QHostAddress)));
    connect(connection, SIGNAL(deleteMember(QHostAddress)),
            this, SLOT(deleteMember(QHostAddress)));
    connect(connection, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(connection, SIGNAL(gotFileNotification(QString,QDateTime)),
            this, SIGNAL(gotFileNotification(QString,QDateTime)));
    connect(connection, SIGNAL(serverFell()), this, SLOT(recoverServer()));
    connect(connection, SIGNAL(gotFileRequest(QString,QDateTime)),
            this, SLOT(respondWithFile(QString,QDateTime)));
    connect(connection, SIGNAL(gotFileResponse(QString,QDateTime,QByteArray)),
            this, SLOT(saveSharedFile(QString,QDateTime,QByteArray)));

    connection->sendPassLoginPriority(pass, login, device_type());
}

void ClientRoom::addMember(FloatServerPriority prior, QHostAddress addr) {
    Member* newMember = new Member(addr, prior);
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

void ClientRoom::sendrequestFile(int index)
{
    const SharedFile * const requested = files.at(index);
    this->sendFileRequest(requested->name, requested->timeStamp);
}

void ClientRoom::addFile(QString n, QDateTime st)
{
    const SharedFile * const file = new SharedFile(n, st);
    this->files.push_back(file);
}

void ClientRoom::respondWithFile(QString fName, QDateTime stamp)
{
    qDebug() << fName << stamp.date() << stamp.time();
    fName.remove(0, 7);
    fName.remove(fName.length()-2, 2);
    QFileInfo info(fName);
    if(info.lastModified() != stamp)
        return;

    if(info.size() > MAX_FILE_SIZE)
        return;

    QFile file(fName);
    if(!file.exists())
        return;

    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    connection->respondFile(fName, stamp, data);
}

void ClientRoom::saveSharedFile(QString fname, QDateTime, QByteArray data)
{
    QString name = fname.split('/').last();
    QString prefix = QFileDialog::getOpenFileName(0, tr("Save file from SharedClipboard as"), "/tmp/"+name);
    QFile file(prefix);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
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
    //delete connection;
}

Member::Member(QHostAddress addr, FloatServerPriority prior)
{
    this->addr = addr;
    this->priority = prior;
}


SharedFile::SharedFile(QString n, QDateTime st):
    name(n), timeStamp(st)
{
}

SharedFile::SharedFile(){}

