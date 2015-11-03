#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QNetworkInterface>
#include <QByteArray>


#include "udpservice.h"
#include "tcpservice.h"
#include "shared_clipboard_exception.h"


class Control : public QObject
{
    Q_OBJECT

    QSharedPointer<UdpService> udpService;
    QSharedPointer<TcpService> tcpService;

    QHostAddress roomHead;
    QString login;
public:
    explicit Control(QString login, QObject *parent = 0) throw(WrongPasswordError);

    void createRoom();
    void joinRoom();

signals:

public slots:
    void sendData(TcpPackage, QByteArray&);
};

#endif // CONTROL_H
