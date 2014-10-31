#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "tcp_package.h"

class Connection : public QObject
{
    Q_OBJECT
private:
    QPair<QString, QString> makeMember(char*);
protected:
    QTcpSocket* socket;
    QByteArray makeBinaryPack(pckg_t type, char* dat, int datsize);
public:
    Connection(QTcpSocket * socket);
    QHostAddress getIpv4();
signals:
    //void gotData(const TcpPackage*);

    void gotText(const QString);
    void gotPass(const QString);
    void gotMember(const QPair<QString, QString>);
    void gotRawData(const char*, const int);
    void gotInvalidPass();
    void gotAdress(const QString);
    void gotRemoveRequest(const QString);

public slots:
    void onData(qint64);
};

#endif // CONNECTION_H
