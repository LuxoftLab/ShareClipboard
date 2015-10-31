#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

#include "constants.h"

class TCPServer : public QTcpServer
{
    Q_OBJECT

public:
    TCPServer();
<<<<<<< HEAD
    ~TCPServer();
=======
    //QHostAddress getLocalAddress();
>>>>>>> origin/interfaces
signals:
    void addMember(qintptr);
    void deleteMember(QHostAddress);
protected:
    void incomingConnection(qintptr) Q_DECL_OVERRIDE;
private:
    QTcpSocket* newConnection = NULL;
};

#endif // TCP_SERVER_H
