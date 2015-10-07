#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include "package.h"

class DataPackage : public TcpPackage
{
    QByteArray data;
    pckg_t type;
public:
    DataPackage(QByteArray&, pckg_t);
    DataPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

#endif // DATAPACKAGE_H
