#include "clipboardtrayicon.h"

void ClipboardTrayIcon::createMenu()
{
    trayIconMenu = new QMenu();

    createRoomAction = new QAction(tr("C&reate room"), this);
    connect(createRoomAction, SIGNAL(triggered()), this, SLOT(createRoom()));
    trayIconMenu->addAction(createRoomAction);

    connectAction = new QAction(tr("C&onnect to room"), this);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectRoom()));
    trayIconMenu->addAction(connectAction);

    trayIconMenu->addSeparator();

    historyAction = new QAction(tr("&History"), this);
    connect(historyAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
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
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    trayIconMenu->addAction(aboutAction);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu->addAction(quitAction);


    icon = new QSystemTrayIcon(this);
    icon->setContextMenu(trayIconMenu);
    icon->setIcon(QIcon(":images/colorful.svg"));
}

void ClipboardTrayIcon::connectMainWindow(MainWindow * mainWindow)
{
    connect(mainWindow, SIGNAL(createRoom()), this, SLOT(createRoom()));
    connect(mainWindow, SIGNAL(connectRoom()), this, SLOT(connectRoom()));
}

ClipboardTrayIcon::ClipboardTrayIcon() : QMainWindow()
{
    createMenu();
    roomDialog = new RoomsListDialog();
}

ClipboardTrayIcon::~ClipboardTrayIcon()
{
    delete icon;
    if(deleteServerAction != NULL)
        delete deleteServerAction;
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
    emit roomListOpened(roomDialog);
    this->icon->show();
}

void ClipboardTrayIcon::connectRoom()
{
    if(roomDialog == NULL){
        roomDialog = new RoomsListDialog(this);
    }
    roomDialog->exec();
}

void ClipboardTrayIcon::createRoom()
{
    if(createRoomDialog == NULL){
        createRoomDialog = new CreateRoomDialog(this);
    }

    if(roomDialog == NULL){
        roomDialog = new RoomsListDialog(this);
    }

    connect(createRoomDialog, SIGNAL(createRoom(QString,QString)),
            this, SIGNAL(serverRoomCreated(QString,QString)));
    createRoomDialog->exec();
}

void ClipboardTrayIcon::showMaximized()
{
    mainwindow = new MainWindow(this);
    connectMainWindow(mainwindow);
    mainwindow->show();
}

void ClipboardTrayIcon::showAbout()
{
    aboutWindow = new About();
    aboutWindow->show();
}

void ClipboardTrayIcon::becomeServer(QString)
{
    icon->setIcon(QIcon(":/images/server.svg"));

    trayIconMenu->removeAction(createRoomAction);
    deleteServerAction = new QAction(tr("D&elete server"), this);
    connect(deleteServerAction, SIGNAL(triggered()),
            this, SIGNAL(deleteServerSignal()));
    trayIconMenu->insertAction(connectAction, deleteServerAction);
}

void ClipboardTrayIcon::stopBeignServer()
{
    icon->setIcon(QIcon(":/images/colorful.svg"));

    trayIconMenu->removeAction(deleteServerAction);
    trayIconMenu->insertAction(connectAction, createRoomAction);
}
