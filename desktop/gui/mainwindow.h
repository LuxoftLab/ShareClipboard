#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMimeData>

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
    void roomCreated(QString name, QString pass, QString login);
    void roomSelected(quint32 addr, QString password);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void addRoom(QString name, qint32 address);
    void chooseRoomClicked();
    void cleapboardChanged(QMimeData * mimeData);
    void connectedToRoom(QString roomName);
    void deleteRoom(qint32 address);
    void newDevicePluged(QString deviceName);
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayMessageClicked();

private:
    Ui::MainWindow * ui;
    QSystemTrayIcon * trayIcon;

    void createTrayIcon();
    bool askForFileDownload(QString fileName);
};

#endif // MAINWINDOW_H
