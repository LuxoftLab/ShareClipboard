#ifndef CLIPBOARDTRAYICON_H
#define CLIPBOARDTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QApplication>
#include <QMenu>

#include "roomslistdialog.h"
#include "passworddialog.h"
#include "createroomdialog.h"
#include "mainwindow.h"
#include "about.h"
#include "settingsdialog.h"

class ClipboardTrayIcon : public QMainWindow
{    
    Q_OBJECT

    QSystemTrayIcon * icon = NULL;
    MainWindow * mainwindow;
    RoomsListDialog * roomDialog = NULL;
    CreateRoomDialog * createRoomDialog = NULL;
    About * aboutWindow = NULL;
    SettingsDialog * settingsDialog = NULL;

    QMenu * trayIconMenu;
    QAction * connectAction;
    QAction * createRoomAction;
    QAction * historyAction;
    //QAction * maximizeAction;
    QAction * stopSharingAction;
    QAction * settingsAction;
    QAction * aboutAction;
    QAction * quitAction;
    QAction * deleteServerAction = NULL;
    QList<QAction*> files;

    QMenu * fileMenu = NULL;

    QString sharingOnString;
    QString sharingOffString;
    QString clientIconPath;
    QString serverIconPath;

    QMap<int, int> fileToIndex; //mainWindowListId -> fileId

    void createMenu();
    void connectMainWindow(MainWindow*);
public:
    ClipboardTrayIcon();
    ~ClipboardTrayIcon();

    void show();
signals:
    void stopSharing();
    void settings();
    void about();
    void quit();
    void toggleSharingSignal();
    void messageClicked();

    void serverRoomCreated(QString, QString);
    void roomListOpened(RoomsListDialog *);
    void deleteServerSignal();
    void requestFile(int);
public slots:
    void connectRoom();
    void createRoom();
    void showMaximized();
    void showAbout();
    void showSettings();
    void toggleSharing();
    void roomCreated(QString);
    void roomJoined(QString, QString);

    void becomeServer(QString);
    void stopBeignServer();
    void showMessage(QString, QString);

    void dataPushedToClipboard(QString text, qint32 id);
    void deleteItemFromList(qint32 id);
    void processRequestFile(int i);
};

#endif // CLIPBOARDTRAYICON_H
