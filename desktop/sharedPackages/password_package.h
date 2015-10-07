#ifndef PASSWORD_PACKAGE_H
#define PASSWORD_PACKAGE_H

#include "package.h"
class PassPackage : public TcpPackage
{
    FloatServerPriority priority;
    QString password;
public:
    PassPackage(QString, FloatServerPriority);
    PassPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

#endif // PASSWORD_PACKAGE_H
