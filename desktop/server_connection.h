#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "constants.h"
#include "connection.h"
#include "tcp_package.h"
#include <unistd.h>


class ServerConnection : public Connection
{
    Q_OBJECT
public:
    ServerConnection(QHostAddress host);
    void sendPassAndLogin(QString password, QString login);
    void deleteMe(QHostAddress address);
public slots:
    void onData();
signals:
    void addMember(QString login, QHostAddress addr);
    void deleteMember(QHostAddress addr);

    void gotInvalidPass();
private:
    void makeMember(char *);
};

#endif // SERVER_CONNECTION_H
