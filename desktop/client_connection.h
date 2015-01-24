#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include "connection.h"

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
signals:
    bool verifyPass(QString pass, ClientConnection * conn);
    void deleteMember(QHostAddress addr);
private slots:
    void emitDeleteMember();
    void onData();
private:
    QByteArray makeBinaryPack(pckg_t, char*, int);
    QByteArray makeBinaryPack(pckg_t, QString);

};

#endif // CLIENT_CONNECTION_H
