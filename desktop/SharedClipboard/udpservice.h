#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>

class UdpService : public QObject
{
    Q_OBJECT
public:
    explicit UdpService(QObject *parent = 0);
    QHostAddress requestRoom();
    QList<QHostAddress> requestMembers();
    ////////////////////////////////////////
    void respondRoom();
    void respondMembers();
signals:

public slots:
private:

};

#endif // UDPSERVICE_H
