#include "client_connection.h"

ClientConnection::ClientConnection(QTcpSocket * socket) : Connection(socket)
{
    this->socket = socket;
    this->login = "login";
    transferFinished = true;

    connect(socket, SIGNAL(disconnected()),
            this, SLOT(emitDeleteMember()));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(onData()));
}

void ClientConnection::sendFail()
{
    FailPackage().write(socket);
}

void ClientConnection::sendMember(QString login, floating_server_priorities priority, QHostAddress addr)
{
    MemberPackage(login, addr, priority).write(socket);
}

void ClientConnection::removeMember(QHostAddress addr)
{
    RemoveMemberPackage(addr).write(socket);
}

QString ClientConnection::getLogin() {
    return login;
}

void ClientConnection::onData(){
    QDataStream in(socket);
    QDataStream infile(&file, QIODevice::ReadOnly);
    if(transferFinished)
    {
        file.clear();
        transferFinished = false;
        in >> currenFiletSize;
        currenFiletSize -= sizeof(packt);
        in >> packt;
    }
    downloadMore(file, socket);
}

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
    hand = TcpPackageFactory().getPackage((pckg_t)packt);
    connect(hand, SIGNAL(gotText(QString)),
            this, SLOT(emitText(QString)));
    connect(hand, SIGNAL(gotImage(QByteArray)),
            this, SLOT(emitImage(QByteArray)));
    connect(hand, SIGNAL(gotPass(QString,floating_server_priorities)),
            this, SLOT(makePass(QString,floating_server_priorities)));

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
