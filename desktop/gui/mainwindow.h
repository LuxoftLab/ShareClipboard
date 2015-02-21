#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QPixmap>
#include <QVector>

#include "gui/roomslistdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fillDevicesList(QList<QString> list);
signals:
    void downloadFile();
    void changeName(QString name);
    void roomListOpened(RoomsListDialog *);
    void pushDataChoosed(qint32);
protected:
    void closeEvent(QCloseEvent * event);
public slots:
    void dataPushedToClipboard(QString text, qint32 id);
    void deleteItemFromList(qint32 id);

    void newDevicePluged(QString deviceName);
    void newNameVerified(QString newName);
private slots:
    void chooseRoomClicked();
    void changeNameClicked();

    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayMessageClicked();

    void clipboardDataListItemDBClicked(QListWidgetItem *listItem);
private:
    Ui::MainWindow * ui;
    QSystemTrayIcon * trayIcon;
    QVector<qint32> dataIdsVector;
    RoomsListDialog *roomDialog;

    void createTrayIcon();
    bool askForFileDownload(QString fileName);
};

#endif // MAINWINDOW_H
