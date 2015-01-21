#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "constants.h"
#include "connection.h"
#include "tcp_package.h"

class ServerConnection : public Connection
{
    Q_OBJECT
public:
    ServerConnection(QHostAddress host);
    int sendPassAndLogin(QString password, QString login);
    void deleteMe(QHostAddress address);
public slots:
    void onData();
signals:
    void addMember(QString login, QHostAddress addr);
    void deleteMember(QHostAddress addr);
    void gotInvalidPass();
private:
    void makeMember(char *);
    void getSocketState(QTcpSocket*);
private slots:
    void emitRemoveMember(char*);
    void throwSocketError(QAbstractSocket::SocketError);
    void connected();


};

#endif // SERVER_CONNECTION_H
