#ifndef FILE_RESPONSE_PACKAGE_H
#define FILE_RESPONSE_PACKAGE_H

#include "package.h"
class FileRespPackage : public TcpPackage
{
    QString fileName;
    QDateTime timeStamp;
    QByteArray data;
public:
    FileRespPackage(QString, QDateTime, QByteArray&);
    FileRespPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};
#endif // FILE_RESPONSE_PACKAGE_H
