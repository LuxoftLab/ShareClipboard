#include "memberpackage.h"


MemberPackage::MemberPackage()
{
    prior = FLOAT_IDLE;
}

MemberPackage::MemberPackage(QHostAddress addr, FloatServerPriority priority)
{
    this->addr = addr;
    this->prior = priority;
}

void MemberPackage::read(QDataStream & in)
{
    qint32 priority;
    in >> priority;
    in >> address;
    emit addMember((FloatServerPriority)priority,
                   QHostAddress(address));
}

void MemberPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << qint32(0) << MEMBER
        << prior
        << addr.toIPv4Address();
    out.device()->seek(0);
    out << qint32(dat.size() - sizeof(qint32));

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}
