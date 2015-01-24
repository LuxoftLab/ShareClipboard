#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "constants.h"
#include "connection.h"
#include "tcp_package.h"


class ServerConnection : public Connection
{

public:
    ServerConnection(QHostAddress host);
    void sendPassAndLogin(QString password, QString login);
    void sendText(QString text);
public slots:
    void addMember(QString login, QHostAddress addr);
    void deleteMember(QHostAddress addr);
private:
    QByteArray makeBinaryPack(pckg_t, char*, int);
};

#endif // SERVER_CONNECTION_H
