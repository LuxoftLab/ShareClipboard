#include "clipboardtrayicon.h"

ClipboardTrayIcon::ClipboardTrayIcon() : QMainWindow(0)
{
    roomDialog = new RoomsListDialog();
    QMenu * trayIconMenu = new QMenu();

    connectAction = new QAction(tr("C&onnect to room"), this);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectRoom()));
    trayIconMenu->addAction(connectAction);

    createRoomAction = new QAction(tr("C&reate room"), this);
    connect(createRoomAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(createRoomAction);

    trayIconMenu->addSeparator();

    historyAction = new QAction(tr("&History"), this);
    connect(historyAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(historyAction);

    trayIconMenu->addSeparator();

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    trayIconMenu->addAction(maximizeAction);

    stopSharingAction = new QAction(tr("&Stop Sharing"), this);
    connect(stopSharingAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(stopSharingAction);

    trayIconMenu->addSeparator();

    settingsAction = new QAction(tr("Se&ttings"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(settingsAction);

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(aboutAction);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu->addAction(quitAction);


    icon = new QSystemTrayIcon();
    icon->setIcon(QIcon(":images/colorful.svg"));
    icon->setContextMenu(trayIconMenu);
    icon->show();
}

ClipboardTrayIcon::~ClipboardTrayIcon()
{
    delete icon;
    delete quitAction;
    delete aboutAction;
    delete settingsAction;
    delete stopSharingAction;
    delete maximizeAction;
    delete historyAction;
    delete createRoomAction;
    delete connectAction;
    delete roomDialog;
}

void ClipboardTrayIcon::connectRoom()
{
    roomDialog->exec();
}

