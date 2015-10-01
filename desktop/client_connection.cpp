#include "client_connection.h"

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;

    connect(socket, SIGNAL(disconnected()),
            this, SLOT(emitDeleteMember()));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(onData()));
}

ClientConnection::ClientConnection(qintptr socketd)
{
    this->socketDescriptor = socketd;
//    this->socket = new QTcpSocket(this);

//    connect(socket, SIGNAL(disconnected()),
//            this, SLOT(emitDeleteMember()));
//    connect(socket, SIGNAL(readyRead()),
//            this, SLOT(onData()));
//    connect(this, &ClientConnection::finished,
//            this, &ClientConnection::deleteLater);
}

void ClientConnection::sendFail()
{
    FailPackage().write(socket);
}

void ClientConnection::sendMember(floating_server_priorities priority, QHostAddress addr)
{
    MemberPackage(addr, priority).write(socket);
}

void ClientConnection::removeMember(QHostAddress addr)
{
    RemoveMemberPackage(addr).write(socket);
}

//void ClientConnection::onData(){
//    QDataStream in(socket);
//    QDataStream infile(&file, QIODevice::ReadOnly);
//    if(transferFinished)
//    {
//        file.clear();
//        transferFinished = false;
//        in >> currenFiletSize;
//        currenFiletSize -= sizeof(packt);
//        in >> packt;
//    }
//    downloadMore(file, socket);
//}

void ClientConnection::sendData(QByteArray arr, pckg_t type)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << type << (qint32)arr.size();
    out.writeRawData(arr.constData(), arr.size());
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    out.device()->seek(4+4+4+arr.size());

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "additional image transfer needed";
    }
}

void ClientConnection::sendFileNotification(QString str, QDateTime stamp)
{
    FileNotificationPackage(str.toUtf8(), stamp).write(socket);
}

void ClientConnection::getFile(QString)
{

}

void ClientConnection::requestFile(QString fname, QDateTime timeStamp)
{
    FileReqPackage(fname, timeStamp).write(socket);
}

void ClientConnection::respondFile(QString fname, QDateTime stamp, QByteArray data)
{
    FileRespPackage(fname, stamp, data).write(socket);
}

void ClientConnection::run()
{
    socket = new QTcpSocket();
    this->socket->setSocketDescriptor(socketDescriptor);
     connect(socket, SIGNAL(disconnected()),
             this, SLOT(emitDeleteMember()));
     connect(socket, SIGNAL(readyRead()),
             this, SLOT(onData()));
     connect(this, &ClientConnection::finished,
             this, &ClientConnection::deleteLater);
}

void ClientConnection::makePass(QString pass, floating_server_priorities priority)
{
    emit(verifyPass(pass, priority, this));
}

void ClientConnection::emitDeleteMember()
{
    emit deleteMember(socket->localAddress());
}

void ClientConnection::downloadMore(QByteArray& whole, QTcpSocket * inSocket)
{
    QByteArray file;
    QDataStream in(&whole, QIODevice::ReadOnly);
    while(inSocket->bytesAvailable() > 0)
    {
        file = inSocket->readAll();
        whole.append(file);
        assert(file.size() > 0);
    }
    if(whole.size() >= currenFiletSize)
    {
        transferFinished = true;
        dispatch(in);
    }
}

void ClientConnection::dispatch(QDataStream& infile)
{
    qint32 packt;
    infile >> packt;
    hand = TcpPackageFactory().getPackage((pckg_t)packt);
    connect(hand, SIGNAL(gotText(QString)),
            this, SLOT(emitText(QString)));
    connect(hand, SIGNAL(gotImage(QByteArray)),
            this, SLOT(emitImage(QByteArray)));
    connect(hand, SIGNAL(gotPass(QString,floating_server_priorities)),
            this, SLOT(makePass(QString,floating_server_priorities)));
    connect(hand, SIGNAL(gotFileNotification(QString, QDateTime)),
            this, SLOT(emitFileNotification(QString, QDateTime)));
    connect(hand, SIGNAL(gotFileReq(QString,QDateTime)),
            this, SLOT(emitFileRequest(QString,QDateTime)));
    connect(hand, SIGNAL(gotFileResp(QString,QDateTime,QByteArray)),
            this, SLOT(emitFileResponse(QString,QDateTime,QByteArray)));

    hand->read(infile);
}

void ClientConnection::emitText(QString text)
{
    emit(onText(text, this));
}

void ClientConnection::emitImage(QByteArray ba)
{
    emit onImage(ba, this);
}

void ClientConnection::emitFileNotification(QString fileName, QDateTime stamp)
{
    emit onFileNotification(fileName, stamp, this);
}

void ClientConnection::emitFileRequest(QString name, QDateTime stamp)
{
    emit onFileRequest(name, stamp, this);
}

void ClientConnection::emitFileResponse(QString str, QDateTime stamp, QByteArray data)
{
    emit onFileRespond(str, stamp, data, this);
}
