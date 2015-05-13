#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

#include "constants.h"

class TCPServer : public QObject
{
    Q_OBJECT

public:
    TCPServer();
    ~TCPServer();
signals:
    void addMember(QTcpSocket * socket);
    void deleteMember(QHostAddress);
private:
    QTcpServer* server;
    QTcpSocket* newConnection;
private slots:
    void newMember();
};

#endif // TCP_SERVER_H
