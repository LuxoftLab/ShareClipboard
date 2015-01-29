#include <QMessageBox>
#include <QtNetwork/QHostInfo>
#include "mainwindow.h"
#include "roomslistdialog.h"
#include "ui_mainwindow.h"
#include "changenamedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createTrayIcon();

    ui->nameLabel->setText("Имя устройства: " + QHostInfo::localHostName());

    connect(ui->changeNamePushButton, SIGNAL(clicked()), this, SLOT(changeNameClicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(chooseRoomClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::askForFileDownload(QString fileName)
{
    QMessageBox msgBox;

    msgBox.setInformativeText(tr("&Do you want to download file ") +  fileName + "?");
    msgBox.setText(tr("&Download file?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec() == QMessageBox::Save;
}

void MainWindow::chooseRoomClicked()
{
    RoomsListDialog * dialog = new RoomsListDialog();
    emit roomListOpened(dialog);
    dialog->exec();

    // TODO implement changing of room after connection
   // ui->pushButton_3->setEnabled(false);
}

void MainWindow::changeNameClicked()
{
    ChangeNameDialog dialog;
    connect(&dialog, SIGNAL(nameChoosed(QString)), this, SIGNAL(changeName(QString)));
    dialog.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void MainWindow::textPushedToClipboard(QString text)
{
    ui->clipboardText->insertItem(0, text);
}

void MainWindow::imagePushedToClipboard(QString imageName)//QPixmap image)
{
    ui->clipboardText->insertItem(0, imageName);
}

void MainWindow::createTrayIcon()
{
    if(!trayIcon->isSystemTrayAvailable())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("System tray is unavailable!"));
        msgBox.exec();
        trayIcon = NULL;
        return;
    }

    QMenu * trayIconMenu = new QMenu(this);

    QAction * minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    trayIconMenu->addAction(minimizeAction);

    QAction * maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    trayIconMenu->addAction(maximizeAction);

    QAction * restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayIconMenu->addAction(restoreAction);

    trayIconMenu->addSeparator();

    QAction * quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("Shared clipboard");
    trayIcon->setIcon(QIcon(":images/communities.svg"));
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(trayMessageClicked()));

    trayIcon->showMessage("Shared Clipboard run", "Click to open main window");
}

void MainWindow::fillDevicesList(QList<QString> list) {
    QList<QString>::iterator i;
    for(i = list.begin(); i != list.end(); i++) {
        ui->listWidget->addItem(*i);
    }
}

void MainWindow::newDevicePluged(QString deviceName)
{
    trayIcon->showMessage(deviceName + tr("& connected to room"), "Shared clipboard");
}

void MainWindow::newNameVerified(QString newName)
{
    ui->nameLabel->setText("Имя устройства: " + newName);
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason != QSystemTrayIcon::Context)
    {
        trayMessageClicked();
    }
}

void MainWindow::trayMessageClicked()
{
    show();
    raise();
    activateWindow();
}


