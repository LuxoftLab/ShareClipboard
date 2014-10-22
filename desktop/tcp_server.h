#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpSocket>

class TCPServer : public QObject
{
    Q_OBJECT

public:
    TCPServer();

signals:
    void addMember(QTcpSocket * socket);
};

#endif // TCP_SERVER_H
