#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Connection : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
public:
    Connection(QTcpSocket * socket);
    QHostAddress getIpv4();
};

#endif // CONNECTION_H
