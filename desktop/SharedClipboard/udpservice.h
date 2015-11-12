#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QHash>
#include <QThread>
#include <QTimer>
#include <QNetworkInterface>
#include <QDataStream>
#include <QHostAddress>


#include "common.h"

class UdpService : public QThread
{
    Q_OBJECT

    QUdpSocket * socket;
    QTimer * aliveTimer;
    QString login;
    QString roomName;
public:
    explicit UdpService(QString login, QObject *parent = 0);
    void iAmAlive(); // login, room, ip-address

    void run() override;

signals:
    void newMember(QString login, QHostAddress addr);
public slots:
    void read();
private:

};

#endif // UDPSERVICE_H
