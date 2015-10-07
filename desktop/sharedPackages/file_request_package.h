#ifndef FILE_REQUEST_PACKAGE_H
#define FILE_REQUEST_PACKAGE_H

#include "package.h"
class FileReqPackage : public TcpPackage
{
    QString fileName;
    QDateTime timeStamp;
public:
    FileReqPackage(QString, QDateTime);
    FileReqPackage();
    ~FileReqPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

#endif // FILE_REQUEST_PACKAGE_H
