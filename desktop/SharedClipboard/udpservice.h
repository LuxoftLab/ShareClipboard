#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QHash>

class UdpService : public QThread
{
    Q_OBJECT
public:
    explicit UdpService(QObject *parent = 0);
    void iAmAlive(); // login, room, ip-address
    void read();

    void run();

signals:

public slots:
private:

};

#endif // UDPSERVICE_H
