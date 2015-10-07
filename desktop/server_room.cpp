#include "server_room.h"

ServerRoom::ServerRoom(QString name, QString pass) : Room(name, pass)
{
    server = new TCPServer();
    connect(server, SIGNAL(addMember(qintptr)),
            this, SLOT(addMember(qintptr)));
    connect(server, SIGNAL(deleteMember(QHostAddress)),
            this, SLOT(deleteMember(QHostAddress)));
}

ServerRoom::~ServerRoom()
{
    for(auto it = fileMetaData.begin(); it != fileMetaData.end(); ++it)
        delete it.value();
    for(auto it = verified.begin(); it != verified.end(); ++it)
        delete it.value();
    for(auto it = notVerified.begin(); it != notVerified.end(); ++it)
        delete it.value();
    delete server;
}

void ServerRoom::addMember(qintptr socket)
{
    ClientConnection *t = new ClientConnection(socket);
    t->run();
    connectConnectionRoom(t, this);
    notVerified.insert(t->getSocket()->peerAddress().toIPv4Address(), t);
}

void ServerRoom::deleteMember(QHostAddress addr)
{
    qint32 ip = addr.toIPv4Address();
    ClientConnection *t = notVerified.value(ip, NULL);
    if(t != NULL)
    {
        notVerified.remove(ip);
        return;
    }
    t = verified.value(ip, NULL);
    if(t != NULL)
    {
        verified.remove(ip);
        for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
        {
            ClientConnection* c = it.value();
            c->removeMember(addr);
        }
        return;
    }
}

bool ServerRoom::verifyPass(QString pass, FloatServerPriority priority, ClientConnection * conn)
{
    if(this->pass != pass)
    {
        qDebug() << "Wrong pass" << pass << this->pass;
        conn->sendFail();
        return false;
    }
    qDebug() << "verified IP:" << conn->localAddress();
    qint32 ip = conn->getIpv4().toIPv4Address();
    notVerified.remove(ip);
    verified.insert(ip, conn);
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendMember(priority, conn->getIpv4());
        conn->sendMember(priority, t->getIpv4());
    }

    return true;
}

void ServerRoom::onText(QString s, ClientConnection * owner)
{
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendData(s.toUtf8(), TEXT);
    }
}

void ServerRoom::onImage(QByteArray im, ClientConnection * const)
{
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendData(im, IMAGE);
    }
}

void ServerRoom::onFileNotification(QString fileName, QDateTime stamp, ClientConnection * const source)
{
    this->saveFileMetaData(fileName, source);
    for(QMap<qint32, ClientConnection*>::Iterator it = verified.begin(); it != verified.end(); it++)
    {
        ClientConnection* t = it.value();
        t->sendFileNotification(fileName, stamp);
    }
}

void ServerRoom::onFileRequest(QString fname, QDateTime timeStamp, ClientConnection * const source)
{
    FileWaitor next = FileWaitor(SharedFile(fname, timeStamp), source);
    fileWaitors.push_front(next);
    source->requestFile(fname, timeStamp);
}

void ServerRoom::onFileResponse(QString fname, QDateTime stamp, QByteArray fileData, ClientConnection * const source)
{
    for(auto it = fileWaitors.begin(); it != fileWaitors.end(); ++it){
        //qDebug() << fname << it->file.name << stamp << it->file.timeStamp << it->sent;
        fname.prepend("file://");
        fname.append("\r\n");
        qDebug() << it->file.name  << fname << (it->file.name == fname);
        if(it->file.name == fname &&
           it->file.timeStamp == stamp &&
           (!it->sent)){
                it->destination->respondFile(fname, stamp, fileData);
                it->sent = true;
        }
    }
}

void ServerRoom::getFile(QString fileName)
{
    ClientConnection * source = getFileOwner(fileName);
    source->getFile(fileName);
}

ClientConnection * ServerRoom::getFileOwner(QString fileName)
{
    return fileMetaData[fileName];
}


FileWaitor::FileWaitor(SharedFile f, ClientConnection * s)
{
    file = f;
    destination = s;
}

void ServerRoom::saveFileMetaData(QString path, ClientConnection * const source)
{
    fileMetaData[path] = source;
}

void ServerRoom::connectConnectionRoom(ClientConnection * t, ServerRoom * s)
{
    connect(t, SIGNAL(verifyPass(QString, FloatServerPriority, ClientConnection* const)),
            s, SLOT(verifyPass(QString, FloatServerPriority, ClientConnection* const)));
    connect(t, SIGNAL(onText(QString, ClientConnection * const)),
            s, SLOT(onText(QString, ClientConnection * const)));
    connect(t, SIGNAL(onImage(QByteArray, ClientConnection * const)),
            s, SLOT(onImage(QByteArray, ClientConnection * const)));
    connect(t, SIGNAL(onFileNotification(QString,QDateTime,ClientConnection*const)),
            s, SLOT(onFileNotification(QString,QDateTime,ClientConnection*const)));
    connect(t, SIGNAL(deleteMember(QHostAddress)),
            s, SLOT(deleteMember(QHostAddress)));
    connect(t, SIGNAL(onFileRequest(QString,QDateTime,ClientConnection*const)),
            s, SLOT(onFileRequest(QString,QDateTime,ClientConnection*const)));
    connect(t, SIGNAL(onFileRespond(QString,QDateTime,QByteArray,ClientConnection*const)),
            s, SLOT(onFileResponse(QString,QDateTime,QByteArray,ClientConnection*const)));
}
