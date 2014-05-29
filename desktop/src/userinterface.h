#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QStringList>
#include <QApplication>

#include "clientlist.h"
#include "logview.h"

class UserInterface : public QObject
{
    Q_OBJECT
public:
    explicit UserInterface(ClientList * peer_list,QObject *parent = 0);
    ~UserInterface();

signals:
    void enableSignal();
    void disableSignal();
    void exitSignal();
private slots:
    void onEnabledTriggered(bool checked);
    void onDisabledTriggered(bool checked);
    void onPeerListChanged(QStringList peers);
    void onExit(bool checked);
private:
    QSystemTrayIcon * m_tray;
    QMenu * m_main_menu;
    QMenu * m_status_menu;
    QMenu * m_peers_menu;
    QAction * m_enable_action;
    QAction * m_disable_action;
    QAction * m_show_log_action;
    QAction * m_exit_action;
    ClientList * m_peer_list;
    LogView * m_log_view;
};

#endif // USERINTERFACE_H
