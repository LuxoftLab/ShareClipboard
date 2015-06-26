#ifndef CLIPBOARDTRAYICON_H
#define CLIPBOARDTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QApplication>

#include "roomslistdialog.h"

class ClipboardTrayIcon : public QMainWindow
{    
    Q_OBJECT

    QSystemTrayIcon * icon;
    RoomsListDialog * roomDialog;

    QAction * connectAction;
    QAction * createRoomAction;
    QAction * historyAction;
    QAction * maximizeAction;
    QAction * stopSharingAction;
    QAction * settingsAction;
    QAction * aboutAction;
    QAction * quitAction;
public:
    ClipboardTrayIcon();
    ~ClipboardTrayIcon();
signals:
    void createRoom();
    void history();
    void maximize();
    void stopSharing();
    void settings();
    void about();
    void quit();
public slots:
    void connectRoom();
};

#endif // CLIPBOARDTRAYICON_H
