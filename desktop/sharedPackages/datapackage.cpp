#include "datapackage.h"


DataPackage::DataPackage(QByteArray& data, pckg_t type)
{
    this->data = data;
    this->type = type;
}

DataPackage::DataPackage()
{
    type = TCP_IDLE;
}

void DataPackage::read(QDataStream & in)
{
    in >> size;
    text = new char[size];
    in.readRawData(text, size);

    emit gotText(QByteArray::fromRawData(text, size));
    emit gotData(QByteArray::fromRawData(text, size), "text/plain");
}

void DataPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << type << (qint32)data.size();
    out.writeRawData(data.constData(), data.size());
    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));
    out.device()->seek(4+4+4+data.size()+1);

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}
