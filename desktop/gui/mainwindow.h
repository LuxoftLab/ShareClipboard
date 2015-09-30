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
    //void connectRoomListDialog();
    void insertItem(QString, int id);
signals:
    void downloadFile();
    void changeName(QString name);
    void roomListOpened(RoomsListDialog *);
    void pushDataChoosed(qint32);
    void settingsChoosed(int, bool);

    void createRoom();
    void deleteRoom();
    void connectRoom();
    void shareClipboard();
    void unshareClipboard();
    void requestFile(int index);
protected:
    void closeEvent(QCloseEvent * event);
public slots:
    void dataPushedToClipboard(QString text, qint32 id);
    void deleteItemFromList(qint32 id);

    void newDevicePluged(QString deviceName);
    void newNameVerified(QString newName);
private slots:
    void connectRoomListDialog();
    void chooseRoomClicked();
    void changeNameClicked();
    void onSettingsClicked();
    void onSettingsAccepted(int value, bool isInKB);

    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayMessageClicked();

    void clipboardDataListItemDBClicked(QListWidgetItem *listItem);

    void on_connectpushButton_clicked();
    void on_changeRoomPushButton_clicked();

    void on_shareccheckBox_toggled(bool checked);

    void on_clipboardText_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow * ui;
    //QSystemTrayIcon * trayIcon;
    QVector<qint32> dataIdsVector;
    RoomsListDialog *roomDialog;
    CreateRoomDialog * createRoomDialog = NULL;

    void createTrayIcon();
   // bool askForFileDownload(QString fileName);
    bool isServer;
};

#endif // MAINWINDOW_H
