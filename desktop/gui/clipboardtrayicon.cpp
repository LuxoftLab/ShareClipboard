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

    historyAction = new QAction(tr("&History/Maximize"), this);
    connect(historyAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    trayIconMenu->addAction(historyAction);

    trayIconMenu->addSeparator();

//    maximizeAction = new QAction(tr("Ma&ximize"), this);
//    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
//    trayIconMenu->addAction(maximizeAction);

    stopSharingAction = new QAction(sharingOffString, this);
    connect(stopSharingAction, SIGNAL(triggered()), this, SLOT(toggleSharing()));
    stopSharingAction->setCheckable(true);
    trayIconMenu->addAction(stopSharingAction);

    trayIconMenu->addSeparator();

    settingsAction = new QAction(tr("Se&ttings"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));
    trayIconMenu->addAction(settingsAction);

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    trayIconMenu->addAction(aboutAction);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu->addAction(quitAction);


    icon = new QSystemTrayIcon(this);

    connect(icon, SIGNAL(messageClicked()),
            this, SIGNAL(messageClicked()));

    icon->setIcon(QIcon(clientIconPath));
    icon->setContextMenu(trayIconMenu);
    icon->showMessage("SharedClipboard", "is running");
}

void ClipboardTrayIcon::dataPushedToClipboard(QString text, qint32 id) {
    int windowId = mainwindow->insertItem(text);
    fileToIndex.insert(windowId,id);
    qDebug() << windowId << id;
}

void ClipboardTrayIcon::deleteItemFromList(qint32 id)
{
    //delete ui->clipboardText->takeItem(dataIdsVector.indexOf(id));
    //dataIdsVector.removeLast();
}

void ClipboardTrayIcon::processRequestFile(int i)
{
    emit requestFile(fileToIndex[i]);
}


void ClipboardTrayIcon::connectMainWindow(MainWindow * mainWindow)
{
    connect(mainWindow, SIGNAL(createRoom()), this, SLOT(createRoom()));
    connect(mainWindow, SIGNAL(connectRoom()), this, SLOT(connectRoom()));
    connect(mainWindow, SIGNAL(requestFile(int)),
            this, SLOT(processRequestFile(int)));
}

ClipboardTrayIcon::ClipboardTrayIcon() : QMainWindow()
{
    clientIconPath = ":/images/client_icon.svg";
    serverIconPath = ":/images/server_icon.svg";
    roomDialog = new RoomsListDialog();
    mainwindow = new MainWindow();
    connectMainWindow(mainwindow);
    mainwindow->show();
    sharingOnString = tr("Enable Sharing");
    sharingOffString = tr("Disable Sharing");
    createMenu();
}

ClipboardTrayIcon::~ClipboardTrayIcon()
{
    delete icon;
    if(deleteServerAction != NULL)
        delete deleteServerAction;
    if(aboutWindow != NULL)
        delete aboutWindow;
    if(settingsDialog != NULL)
        delete settingsDialog;
    delete quitAction;
    delete aboutAction;
    delete settingsAction;
    delete stopSharingAction;
    //delete maximizeAction;
    delete historyAction;
    delete createRoomAction;
    delete connectAction;
    delete trayIconMenu;
    delete mainwindow;
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
//    mainwindow = new MainWindow(this);
//    this->connectMainWindow(mainwindow);
    mainwindow->show();
}

void ClipboardTrayIcon::showAbout()
{
    aboutWindow = new About();
    aboutWindow->show();
}

void ClipboardTrayIcon::showSettings()
{
    if(settingsDialog == NULL){
        settingsDialog = new SettingsDialog();
        connect(settingsDialog, SIGNAL(settingsAccepted(qint32,QString)),
                this, SIGNAL(settingsAccepted(qint32,QString)));
    }
    settingsDialog->show();
}

void ClipboardTrayIcon::toggleSharing()
{
    if(stopSharingAction->text() == sharingOffString){
        stopSharingAction->setText(sharingOnString);
    }
    else if(stopSharingAction->text() == sharingOnString){
        stopSharingAction->setText(sharingOffString);
    }
    emit toggleSharingSignal();
}

void ClipboardTrayIcon::roomCreated(QString roomName)
{
    icon->showMessage(tr("Created room"), roomName);
}

void ClipboardTrayIcon::roomJoined(QString roomName, QString serverIP)
{
    icon->showMessage(tr("You joined room \"")+roomName+"\"", tr("on the host ")+serverIP);
}

void ClipboardTrayIcon::becomeServer(QString)
{
    icon->setIcon(QIcon(serverIconPath));

    trayIconMenu->removeAction(createRoomAction);
    deleteServerAction = new QAction(tr("D&elete server"), this);
    connect(deleteServerAction, SIGNAL(triggered()),
            this, SIGNAL(deleteServerSignal()));
    trayIconMenu->insertAction(connectAction, deleteServerAction);
}

void ClipboardTrayIcon::stopBeignServer()
{
    icon->setIcon(QIcon(clientIconPath));

    trayIconMenu->removeAction(deleteServerAction);
    trayIconMenu->insertAction(connectAction, createRoomAction);
}

void ClipboardTrayIcon::showMessage(QString title, QString body)
{
    icon->showMessage(title, body);
}
