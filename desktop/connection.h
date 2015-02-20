#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Connection : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket* socket;
public:
    Connection(QTcpSocket * socket);
    QHostAddress getIpv4();
public slots:
    //virtual void sendText(QString text) = 0;
};

#endif // CONNECTION_H
