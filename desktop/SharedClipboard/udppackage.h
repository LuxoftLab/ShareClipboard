#ifndef UDPPACKAGE_H
#define UDPPACKAGE_H

#include <QByteArray>
#include <QString>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDataStream>

#include "common.h"

enum class UdpPackage : int
{
      ALIVE,
};


class IPackage
{
public:
    IPackage();
    virtual void write(QUdpSocket*) = 0;
    virtual void read(QDataStream const &) = 0;
protected:

};

class AlivePackage : public IPackage
{
    QString login;
    QString room;
    QList<QHostAddress> ip_addresses;
public:
    AlivePackage(QString, QString, QList<QHostAddress>);
    void write(QUdpSocket*);
    void read(QDataStream const &);
};

#endif // UDPPACKAGE_H
