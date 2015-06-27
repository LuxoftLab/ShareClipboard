#ifndef CLIPBOARDTRAYICON_H
#define CLIPBOARDTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QApplication>

#include "roomslistdialog.h"
#include "passworddialog.h"
#include "createroomdialog.h"

class ClipboardTrayIcon : public QMainWindow
{    
    Q_OBJECT

    QSystemTrayIcon * icon;
    RoomsListDialog * roomDialog = NULL;
    CreateRoomDialog * createRoomDialog;

    QMenu * trayIconMenu;
    QAction * connectAction;
    QAction * createRoomAction;
    QAction * historyAction;
    QAction * maximizeAction;
    QAction * stopSharingAction;
    QAction * settingsAction;
    QAction * aboutAction;
    QAction * quitAction;

    void createMenu();
public:
    ClipboardTrayIcon();
    ~ClipboardTrayIcon();

    void show();
signals:
    void history();
    void maximize();
    void stopSharing();
    void settings();
    void about();
    void quit();

    void serverRoomCreated(QString, QString);
    void roomListOpened(RoomsListDialog *);
public slots:
    void connectRoom();
    void createRoom();
};

#endif // CLIPBOARDTRAYICON_H
