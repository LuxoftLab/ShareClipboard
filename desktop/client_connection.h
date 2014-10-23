#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H


#include "connection.h"

class ClientConnection : public Connection
{
    QString login;
public:
    ClientConnection(QTcpSocket * socket);
    void sendFail();
    void sendMember(QString login, QHostAddress addr);
    void removeMember(QHostAddress addr);
    QString getLogin();
signals:
    bool verifyPass(QString pass, ClientConnection * conn);
};

#endif // CLIENT_CONNECTION_H
