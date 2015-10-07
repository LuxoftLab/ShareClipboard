#include "removemember.h"


RemoveMemberPackage::RemoveMemberPackage()
{

}

RemoveMemberPackage::RemoveMemberPackage(QHostAddress toRemove)
{
    this->removeAddr = toRemove;
}

void RemoveMemberPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << (qint32)(sizeof(qint32)) << REMOVE << removeAddr.toIPv4Address();

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}

void RemoveMemberPackage::read(QDataStream &)
{

}
