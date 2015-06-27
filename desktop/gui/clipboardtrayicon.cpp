#include "clipboardtrayicon.h"

void ClipboardTrayIcon::createMenu()
{
    trayIconMenu = new QMenu();

    connectAction = new QAction(tr("C&onnect to room"), this);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectRoom()));
    trayIconMenu->addAction(connectAction);

    createRoomAction = new QAction(tr("C&reate room"), this);
    connect(createRoomAction, SIGNAL(triggered()), this, SLOT(createRoom()));
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
}

ClipboardTrayIcon::ClipboardTrayIcon() : QMainWindow()
{
    createMenu();
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
    delete trayIconMenu;
}

void ClipboardTrayIcon::show()
{
    this->icon->show();
}

void ClipboardTrayIcon::connectRoom()
{
    if(roomDialog == NULL){
        roomDialog = new RoomsListDialog(this);
        emit roomListOpened(roomDialog);
    }


    roomDialog->exec();
}

void ClipboardTrayIcon::createRoom()
{
    createRoomDialog = new CreateRoomDialog(this);
    roomDialog = new RoomsListDialog(this);
    emit roomListOpened(roomDialog);

    connect(createRoomDialog, SIGNAL(createRoom(QString,QString)),
            this, SIGNAL(serverRoomCreated(QString,QString)));
    createRoomDialog->exec();
}
