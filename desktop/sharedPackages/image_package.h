#ifndef IMAGE_PACKAGE_H
#define IMAGE_PACKAGE_H

#include "package.h"
class ImagePackage : public TcpPackage
{
public:
    ImagePackage();
    void write(QTcpSocket *);
    void read(QDataStream &);
};

#endif // IMAGE_PACKAGE_H
