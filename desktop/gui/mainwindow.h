#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMimeData>
#include <QHash>

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

protected:
    void closeEvent(QCloseEvent * event);

public slots:
    void cleapboardChanged(QMimeData * mimeData);
    void newDevicePluged(QString deviceName);
    void newNameVerified(QString newName);

private slots:
    void chooseRoomClicked();
    void changeNameClicked();

    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayMessageClicked();

private:
    Ui::MainWindow * ui;
    QSystemTrayIcon * trayIcon;

    void createTrayIcon();
    bool askForFileDownload(QString fileName);
};

#endif // MAINWINDOW_H
