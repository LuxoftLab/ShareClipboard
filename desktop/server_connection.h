#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "constants.h"
#include "connection.h"
#include "tcp_package.h"

#include <assert.h>
#include <QByteArray>

class ServerConnection : public Connection
{
    Q_OBJECT
public:
    ServerConnection(QHostAddress host);
    int sendPassAndLogin(QString password, QString login);
public slots:
    void onData();
signals:
    void addMember(QString login, QHostAddress addr);
    void deleteMember(QHostAddress addr);
private:
    void makeMember(char *);
private slots:
    void emitRemoveMember(char*);
    void connected();
};

#endif // SERVER_CONNECTION_H
