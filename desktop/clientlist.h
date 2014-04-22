#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QObject>
#include <QHostAddress>
#include <QMap>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QMenu>
#include <QDebug>

#include "common.h"
#include "logger.h"

class ClientList : public QObject
{
    Q_OBJECT
public:
    explicit ClientList(QObject *parent = 0);
    QMenu * contextMenu();
signals:
    void sendHello(QHostAddress host);
    void sendAreYouHere(QHostAddress host);
public slots:
    void clientFound(QHostAddress host, QString name);
    void helloReceived(QHostAddress host, QString name);
    void onTimerTriggered();
private:

    void updatePeerMenu();

    QMap<QString,QHostAddress> m_clients;
    QList<QString> m_marked_for_delete;
    QTimer m_timer;
    QMutex m_guard;
    QMenu m_menu;
    QMenu m_peers_list;
};

#endif // CLIENTLIST_H
