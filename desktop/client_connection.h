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
signals:
    void verifyPass(QString pass, ClientConnection * const);
    void deleteMember(QHostAddress addr);
private slots:
    void onData();
    void emitDeleteMember();
private:
    void makeMember(QDataStream&);
    void makePass(QDataStream&);
    QHostAddress makeHostAdress(char*);

};

#endif // CLIENT_CONNECTION_H
