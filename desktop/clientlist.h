#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QObject>
#include <QHostAddress>
#include <QMap>
#include <QDebug>

#include "logger.h"

class ClientList : public QObject
{
    Q_OBJECT
public:
    explicit ClientList(QObject *parent = 0);
signals:
    void sendHello(QHostAddress host);
public slots:
    void clientFound(QHostAddress host, QString name);
private:
    QMap<QString,QHostAddress> m_clients;
};

#endif // CLIENTLIST_H
