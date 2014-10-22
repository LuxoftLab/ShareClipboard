#ifndef UDP_SERVICE_H
#define UDP_SERVICE_H

#include <QObject>
#include <QHostAddress>

class UDPService : public QObject
{
    Q_OBJECT

public:
    UDPService();
    bool initListener();
    void getRooms();
    void sendRoom(QString name);
    void notifyAboutRoom(QString name);
signals:
    void addRoom(QString name, QHostAddress host);
};

#endif // UDP_SERVICE_H
