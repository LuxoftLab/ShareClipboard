#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QHash>
#include <QThread>

class UdpService : public QThread
{
    Q_OBJECT

    QUdpSocket * socket;
public:
    explicit UdpService(QObject *parent = 0);
    void iAmAlive(); // login, room, ip-address
    void read();

    void run() override;

signals:
    void newMember(QString login, QHostAddress addr);
public slots:
private:

};

#endif // UDPSERVICE_H
