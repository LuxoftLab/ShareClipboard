#include "file_request_package.h"

FileReqPackage::FileReqPackage(QString name, QDateTime time)
                : fileName(name)
{
    timeStamp = time;
}

FileReqPackage::FileReqPackage()
{

}

FileReqPackage::~FileReqPackage()
{
    //delete timeStamp;
}

void FileReqPackage::read(QDataStream & in)
{
    qint64 rawTimeStamp;

    in >> rawTimeStamp;
    timeStamp = QDateTime();
    timeStamp.setMSecsSinceEpoch(rawTimeStamp);
    in >> size;
    text = new char[size];
    in.readRawData(text, size);

    qDebug() << timeStamp << size << text;

    emit gotFileReq(QString::fromUtf8(text, size), timeStamp);
}

void FileReqPackage::write(QTcpSocket * socket)
{
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);

    out << qint32(0) << FILEREQ << timeStamp.toMSecsSinceEpoch()
    << (qint32)fileName.toUtf8().size();
    out.writeRawData(fileName.toUtf8().constData(), fileName.toUtf8().size());

    out.device()->seek(0);
    out << (qint32)(dat.size()-sizeof(qint32));

//    out.device()->seek(PCKG_SZ_FIELD_SZ+
//                       PCKG_TYPE_FIELD_SZ+
//                       BYTE_ARR_SZ_FIELD_SZ+
//                       data.size()+1);

    if(socket->write(dat) < dat.size())
    {
        qDebug() << "No data written";
    }
}
