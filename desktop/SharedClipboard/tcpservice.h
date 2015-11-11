#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QSharedPointer>
#include <QThread>

#include "tcppackages.h"
#include "common.h"
#include "encoder.h"

class TcpService : public QObject
{
    Q_OBJECT

    QTcpSocket * ownSocket = NULL;
    QTcpServer * ownServer = NULL;
    QSharedPointer<Encoder> encoder;

public:
    explicit TcpService(QObject *parent = 0);

    void createServer();

    void connectSocket(QHostAddress dest);
    void authenticate(QString);
    void addRoomMembers(QList<QHostAddress>);
    ////////////////////////////////////////

    void sendAuthAnswer();

    ////////////////////////////////////////
    void sendData(TcpPackage, QByteArray&);

signals:

public slots:

private slots:
    void read();
    void registerConnection();
//    void removeMember();

private:
    void write(TcpPackage type, QByteArray & data, QTcpSocket *);

};

#endif // TCPSERVICE_H
