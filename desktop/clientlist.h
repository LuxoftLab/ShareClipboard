#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QObject>
#include <QHostAddress>
#include <QMap>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QMenu>
#include <QElapsedTimer>
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
    void sendClipboard(QHostAddress peer,QString text);
public slots:
    void clientFound(QHostAddress host, QString name);
    void helloReceived(QHostAddress host, QString name);
    void onSendClipboard(QString text);
    void onTimerTriggered();
    void onEnable();
    void onDisable();
private:

    void updatePeerMenu();

    QMap<QString,QHostAddress> m_clients;
    QMap<QString,QElapsedTimer *> m_elapsed_timers;
    QTimer m_timer;
    QMutex m_guard;
    QMenu m_menu;
    QMenu m_peers_list;
};

#endif // CLIENTLIST_H
