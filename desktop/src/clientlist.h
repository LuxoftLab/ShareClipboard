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
    void sendAreYouHere(QHostAddress host);
    void sendClipboard(QHostAddress peer,QString text);
public slots:
    void clientFound(QHostAddress host, QString name);
    void onSendClipboard(QString text);
    void onEnable();
    void onDisable();
    void resetTimerForPeer(QHostAddress peer);

private slots:
    void onTimerTriggered();
private:
    void updatePeerMenu();

    QMap<QHostAddress,QString> m_clients;
    QMap<QHostAddress,QElapsedTimer *> m_elapsed_timers;
    QTimer m_timer;
    QMutex m_guard;
    QMenu m_menu;
    QMenu m_peers_list;
};

#endif // CLIENTLIST_H
