#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class TCPServer : public QObject
{
    Q_OBJECT

public:
    TCPServer();
    QHostAddress getLocalAddress();
signals:
    void addMember(QTcpSocket * socket);
    void deleteMember(QHostAddress addr);
};

#endif // TCP_SERVER_H
