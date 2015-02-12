#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include "connection.h"
#include "tcp_package.h"
#include <assert.h>

class ClientConnection : public Connection
{
    Q_OBJECT
    QString login;
signals:
    void verifyPass(QString pass, ClientConnection * const);
    void onText(QString, ClientConnection * const);
public:
    ClientConnection(QTcpSocket * socket);
    ~ClientConnection();
    void sendFail();
    void sendMember(QString login, QHostAddress addr);
    void removeMember(QHostAddress addr);
    QString getLogin();
    QHostAddress makeHostAdress(char*);
    void sendText(QString);
public slots:
    void onData();
    void emitText(QDataStream&);
private:
    void makePass(QDataStream&);
};

#endif // CLIENT_CONNECTION_H
