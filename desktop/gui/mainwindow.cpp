#include <QMessageBox>
#include "mainwindow.h"
#include "roomslistdialog.h"
#include "createroomdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createTrayIcon();

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(chooseRoomClicked()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(createRoomClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::askForFileDownload(QString fileName) {
    QMessageBox msgBox;

    msgBox.setInformativeText(tr("&Do you want to download file ") +  fileName + "?");
    msgBox.setText(tr("&Download file?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec() == QMessageBox::Save;
}

void MainWindow::chooseRoomClicked() {
    emit querryRoomsList();

    /* TEST */
    QString tmp = "a";
    for(int i = 0; i < 10; i++) {
        rooms.append(tmp, i);
        tmp += tmp;
    }
    /* TEST*/

    // getRooms()
    RoomsListDialog dialog(rooms.keys(), this);
    connect(&dialog, SIGNAL(roomChoosed(QString,QString), this, SLOT(onRoomChoosed(QString,QString)));
    dialog.exec();
}

void MainWindow::cleapboardChanged(QMimeData * mimeData) {
    if (mimeData->hasImage()) {
        ui->cleapboardText->setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasUrls()) {
        ui->cleapboardText->setText("url: " + mimeData->text());
        if(askForFileDownload(mimeData->text())) {
            emit downloadFile();
        }
    } else if (mimeData->hasText()) {
        ui->cleapboardText->setText(mimeData->text());
    }  else {
        ui->cleapboardText->setText(tr("Cannot display data"));
    }
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

void MainWindow::addRoom(QString name, qint32 address)
{

}

void MainWindow::connectedToRoom(qint32 address)
{
    ui->roomLabel->setText(tr("&Your room: ") + address);
    trayIcon->showMessage(tr("&You connected to room") + address, "Shared cleapboard");
}

void MainWindow::onRoomChoosed(QString roomName, QString password)
{
    emit roomSelected(rooms.take(roomName), password, "SOME LOGIN");
}

void MainWindow::createRoomClicked()
{
    CreateRoomDialog dialog(this);
    dialog.exec();
}

void MainWindow::deleteRoom(qint32 address)
{

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
    trayIcon->setToolTip("Shared cleapboard");
    trayIcon->setIcon(QIcon(":images/communities.svg"));
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                             this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(trayMessageClicked()));

    trayIcon->showMessage("Shared Cleapboard run", "Click to open main window");
}

void MainWindow::fillDevicesList(QList<QString> list) {
    QList<QString>::iterator i;
    for(i = list.begin(); i != list.end(); i++) {
        ui->listWidget->addItem(*i);
    }
}

void MainWindow::newDevicePluged(QString deviceName)
{
    trayIcon->showMessage(deviceName + tr("& connected to room"), "Shared cleapboard");
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


