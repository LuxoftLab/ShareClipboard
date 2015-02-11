#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include "connection.h"
#include "tcp_package.h"
#include <assert.h>

class ClientConnection : public Connection
{
    Q_OBJECT
    QString login;
public:
    ClientConnection(QTcpSocket * socket);
    void sendFail();
    void sendMember(QString login, QHostAddress addr);
    void removeMember(QHostAddress addr);
    QString getLogin();
    void sendText(QString text);
    void makePass(QDataStream&);
    QHostAddress makeHostAdress(char*);
public slots:
    void onData();
signals:
    void verifyPass(QString pass, ClientConnection * const);
};

#endif // CLIENT_CONNECTION_H
