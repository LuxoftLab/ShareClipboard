#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "constants.h"
#include "connection.h"
#include "tcp_package.h"

#include <assert.h>
#include <QByteArray>
#include <QBuffer>
#include <QImage>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

class ServerConnection : public Connection
{
    Q_OBJECT
public:
    ServerConnection(QHostAddress host);
    int sendPassAndLogin(QString password, QString login);
    void sendText(QString text);
    void sendImage(QImage);

    ServerConnectionHandler* hand;
public slots:
    void onData();
signals:
    void addMember(QString login, QHostAddress addr);
    void deleteMember(QHostAddress addr);
    void gotInvalidPass();
    void gotPass(QString);
    void gotText(QString);
    void gotImage(QByteArray);
    void gotData(QByteArray, QString);
private:
    void makeMember(QDataStream&);
    void removeMember(QDataStream&);
    void makeText(QDataStream&);
    void makeImage(QDataStream&);
private slots:
    void connected();
};

#endif // SERVER_CONNECTION_H
