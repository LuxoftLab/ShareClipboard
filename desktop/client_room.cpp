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


void ClientRoom::setMaxFileSize(const qint32 &value)
{
    maxFileSize = value * 1024 * 1024;
}


void ClientRoom::setDefaultPath(const QString &value)
{
    defaultPath = value;
}
FloatServerPriority ClientRoom::device_type()
{
    return PC;
}

ClientRoom::ClientRoom(QString name, QHostAddress host) : Room(name, "")
{
    this->host = host;
    connection = NULL;
    maxFileSize = MAX_FILE_SIZE * 1024 * 1024; // in MB
    defaultPath = "/tmp/";
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
//    qDebug() << floating_server_candidates.first()->addr << connection->localAddress();

//    for(auto it = floating_server_candidates.begin(); it < floating_server_candidates.end(); ++it)
//        qDebug() << "in list:" << (*it)->addr.toIPv4Address() << " ";
//    qDebug() << "local" << connection->localAddress().toIPv4Address();

    //if there's at least one host here
    //and it's me
    //I should become a server!!!
    if(floating_server_candidates.size() > 0 &&
       floating_server_candidates.first()->addr.toIPv4Address() == connection->localAddress().toIPv4Address())
        emit newFloatingServer(host);
//    for(auto it = floating_server_candidates.begin(); it < floating_server_candidates.end(); ++it)
//        qDebug() << "in list:" << (*it)->addr.toIPv4Address() << " ";
//    qDebug() << "local" << connection->localAddress().toIPv4Address();
//    qDebug() << floating_server_candidates.size();
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
    emit notificateAboutFile(n, st, files.size()-1);
}

void ClientRoom::respondWithFile(QString fName, QDateTime stamp)
{
    qDebug() << fName << stamp.date() << stamp.time();
    fName.remove(0, 7);
    fName.remove(fName.length()-2, 2);
    QFileInfo info(fName);
    if(info.lastModified() != stamp)
        return;

    if(info.size() > maxFileSize)
    {
        emit tooBigFile(fName);
        qDebug() << info.size();
        return;
    }

    QFile file(fName);
    if(!file.exists())
        return;

    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    qDebug() << data.size();
    connection->respondFile(fName, stamp, data);
}

void ClientRoom::saveSharedFile(QString fname, QDateTime, QByteArray data)
{
    QString name = fname.split('/').last();
    QString prefix = QFileDialog::getOpenFileName(0, tr("Save file from SharedClipboard as"), defaultPath+name);
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

bool SharedFile::operator==(const SharedFile & other)
{
    return this->name == other.name && this->timeStamp == other.timeStamp;
}

bool SharedFile::operator<(const SharedFile & other) const
{
    return (timeStamp == other.timeStamp ?
                timeStamp < other.timeStamp :
                name < other.name);
}

