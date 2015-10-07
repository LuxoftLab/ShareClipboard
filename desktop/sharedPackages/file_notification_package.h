#ifndef FILE_NOTIFICATION_PACKAGE_H
#define FILE_NOTIFICATION_PACKAGE_H

#include "package.h"
class FileNotificationPackage : public TcpPackage
{
    QByteArray data;
    //QHostAddress sourceAddress;
    QDateTime timeStamp;

public:
    FileNotificationPackage();
    FileNotificationPackage(QByteArray, QDateTime);

    void read(QDataStream &);
    void write(QTcpSocket *);
};

#endif // FILE_NOTIFICATION_PACKAGE_H
