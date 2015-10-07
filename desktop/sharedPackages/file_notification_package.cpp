#include "file_notification_package.h"


FileNotificationPackage::FileNotificationPackage()
{

}

FileNotificationPackage::FileNotificationPackage(QByteArray data, QDateTime stamp)
{
    this->data = data;
    //this->sourceAddress = source;
    this->timeStamp = stamp;
}

void FileNotificationPackage::read(QDataStream & in)
{
    in >> size;
    text = new char[size];
    in.readRawData(text, size);
    qint64 stamp;
    in >> stamp;
//    qint32 sourceAddress;
//    in >> sourceAddress;

    QString fileName = QString::fromUtf8(text, size);
    //QHostAddress adr(sourceAddress);

    qDebug() << fileName <<  size << (QDateTime().fromMSecsSinceEpoch(stamp));

    emit gotFileNotification(QString::fromUtf8(text, size),
                             QDateTime().fromMSecsSinceEpoch(stamp));
}

void FileNotificationPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << FILENOTIF << (qint32)data.size();
    out.writeRawData(data.constData(), data.size());
    out << timeStamp.toMSecsSinceEpoch();
    //out << sourceAddress.toIPv4Address();
    out.device()->seek(0);
    out << (qint32)(dat.size()-sizeof(qint32));

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}
