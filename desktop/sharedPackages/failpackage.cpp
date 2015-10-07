#include "failpackage.h"

void FailPackage::read(QDataStream &)
{

}

void FailPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << INVALID_PASS;

    if(socket->write(dat) == 0)
    {
        qDebug() << "No data written";
    }
}
