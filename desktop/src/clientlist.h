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
signals:
    void sendAreYouHere(QHostAddress host);
    void sendClipboard(QHostAddress peer,QString text);
    void peerListChanged(QStringList peers);
public slots:
    void clientFound(QHostAddress host, QString name);
    void onSendClipboard(QString text);
    void onEnable();
    void onDisable();
    void resetTimerForPeer(QHostAddress peer);

private slots:
    void onTimerTriggered();
private:
    void updatePeerList();

    QMap<QHostAddress,QString> m_clients;
    QMap<QHostAddress,QElapsedTimer *> m_elapsed_timers;
    QList<QHostAddress> m_possible_timeout;
    QTimer m_timer;
    QMutex m_guard;
};

#endif // CLIENTLIST_H
