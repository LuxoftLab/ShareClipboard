#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "connection.h"

class ServerConnection : public Connection
{

public:
    ServerConnection();
    void sendPassAndLogin(QString password, QString login);
signals:
    void addMember(QString login, QHostAddress addr);
};

#endif // SERVER_CONNECTION_H
