#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include "connection.h"
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
    QByteArray makeBinaryPack(pckg_t, char*, int);
    QByteArray makeBinaryPack(pckg_t, QString);
    QByteArray makeBinaryPack(pckg_t, qint32);
    void makeMember(QDataStream&);
    void makePass(QDataStream&);
    QHostAddress makeHostAdress(char*);

};

#endif // CLIENT_CONNECTION_H
