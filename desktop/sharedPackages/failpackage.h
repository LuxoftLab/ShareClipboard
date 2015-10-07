#ifndef FAILPACKAGE_H
#define FAILPACKAGE_H

#include "package.h"

class FailPackage : public TcpPackage
{
public:
    void read(QDataStream &);
    void write(QTcpSocket *);
};

#endif // FAILPACKAGE_H
