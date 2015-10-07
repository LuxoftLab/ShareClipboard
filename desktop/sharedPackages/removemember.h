#ifndef REMOVEMEMBER_H
#define REMOVEMEMBER_H

#include "package.h"
class RemoveMemberPackage : public TcpPackage
{
    QHostAddress removeAddr;
public:
    RemoveMemberPackage();
    RemoveMemberPackage(QHostAddress);
    void write(QTcpSocket *);
    void read(QDataStream &);
};

#endif // REMOVEMEMBER_H
