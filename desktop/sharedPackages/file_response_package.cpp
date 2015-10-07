#include "file_response_package.h"

FileRespPackage::FileRespPackage(QString fname, QDateTime timeSt, QByteArray & dat)
                : fileName(fname), timeStamp(timeSt), data(dat)
{

}

FileRespPackage::FileRespPackage()
{
}

void FileRespPackage::read(QDataStream & in)
{
    qint64 stamp;

    in >> stamp;
    timeStamp.setMSecsSinceEpoch(stamp);

    in >> size;
    text = new char[size];
    in.readRawData(text, size);

    in >> size2;
    char * data = new char[size2];
    in.readRawData(data, size2);

    emit gotFileResp(QString::fromUtf8(text, size),
                     timeStamp,
                     QByteArray(data, size2));
}

void FileRespPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << FILERESP << timeStamp.toMSecsSinceEpoch()
    << (qint32)fileName.toUtf8().size();
    out.writeRawData(fileName.toUtf8().constData(), fileName.toUtf8().size());
    out << (qint32)data.size();
    out.writeRawData(data.constData(), data.size());

    out.device()->seek(0);
    out << (qint32)(dat.size() - sizeof(qint32));

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}


