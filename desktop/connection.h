#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "tcp_package.h"

class Connection : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket* socket;
    QByteArray makeBinaryPack(pckg_t type, char* dat, int datsize);
public:
    Connection(QTcpSocket * socket);
    QHostAddress getIpv4();
signals:
    void gotData(const TcpPackage*);
public slots:
    void onData(qint64);
};

#endif // CONNECTION_H
