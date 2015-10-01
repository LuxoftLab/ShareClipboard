#include "tcp_package.h"

TcpPackage *TcpPackageFactory::getPackage(pckg_t type)
{
    switch(type){
    case MEMBER:
        return new MemberPackage();
    case TEXT:
        return new DataPackage();
    case REMOVE:
        return new RemoveMemberPackage();
    case IMAGE:
        return new ImagePackage();
    case FILENOTIF:
        return new FileNotificationPackage();
    case FILEREQ:
        return new FileReqPackage();
    case FILERESP:
        return new FileRespPackage();
    case PASS:
        return new PassPackage();
   default: throw type;
    }
}

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

PassPackage::PassPackage(QString pass, floating_server_priorities prior)
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
    emit gotPass(QString::fromUtf8(pwd, pwdsz), (floating_server_priorities)priority);
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

MemberPackage::MemberPackage()
{
    prior = FLOAT_IDLE;
}

MemberPackage::MemberPackage(QHostAddress addr, floating_server_priorities priority)
{
    this->addr = addr;
    this->prior = priority;
}

void MemberPackage::read(QDataStream & in)
{
    qint32 priority;
    in >> priority;
    in >> address;
    emit addMember((floating_server_priorities)priority,
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

ImagePackage::ImagePackage()
{

}

void ImagePackage::write(QTcpSocket *)
{

}

void ImagePackage::read(QDataStream & in)
{
    in >> size;
    image = new char[size];
    in.readRawData(image,size);

    emit gotData(QByteArray(image, size), "image/png");
    emit gotImage(QByteArray(image, size));
}


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


TcpPackage::~TcpPackage()
{

}

TcpPackage::TcpPackage()
{

}


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
