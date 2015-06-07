#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

#include "tcp_package.h"

class Connection : public QObject
{
    Q_OBJECT
protected:
    void downloadMore(QByteArray& whole, QTcpSocket * inSocket);
    virtual void dispatch(QDataStream&) = 0;

    QTcpSocket* socket;
    bool transferFinished;
    qint32 currenFiletSize;
    qint32 packt;
    QByteArray file;
public:
    Connection(QTcpSocket * socket);
    QHostAddress getIpv4();
};

#endif // CONNECTION_H
