#ifndef MEMBERPACKAGE_H
#define MEMBERPACKAGE_H

#include "sharedPackages/package.h"

class MemberPackage : public TcpPackage
{
    QHostAddress addr;
    FloatServerPriority prior;
public:
    MemberPackage();
    MemberPackage(QHostAddress, FloatServerPriority);
    void read(QDataStream &);
    void write(QTcpSocket *);
};


#endif // MEMBERPACKAGE_H
