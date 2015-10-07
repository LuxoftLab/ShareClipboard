#include "password_package.h"


PassPackage::PassPackage(QString pass, FloatServerPriority prior)
{
    this->password = pass;
    this->priority = prior;
}

PassPackage::PassPackage()
{
    this->priority = FLOAT_IDLE;
}

void PassPackage::read(QDataStream & in)
{
    qint32 priority;
    qint32 pwdsz;
    in >> priority;
    in >> pwdsz;
    char* pwd = new char[pwdsz];
    in >> pwd;
    emit gotPass(QString::fromUtf8(pwd, pwdsz), (FloatServerPriority)priority);
}

void PassPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << qint32(0) << PASS
        << (qint32)priority
        << (qint32)password.toUtf8().size()
        << password.toUtf8().data();
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    socket->write(dat);
}
