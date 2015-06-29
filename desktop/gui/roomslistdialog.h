#ifndef ROOMSLISTDIALOG_H
#define ROOMSLISTDIALOG_H

#include "ui_roomslistdialog.h"
#include "passworddialog.h"
#include "createroomdialog.h"
#include <assert.h>

#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QHostAddress>

namespace Ui {
class RoomsListDialog;
}

class RoomsListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomsListDialog(QWidget * parent = 0);
    ~RoomsListDialog();

signals:
    void roomChoosed(qint32 roomIp, QString password);
    void deleteServerRoom();
    void newRoomCreated(QString, QString);

public slots:
    void accept();
    void addRoom(QString name, qint32 id);
    void deleteRoom(QString name);
    void onServerIsUp(QString serverName);

private slots:
    void onListItemDoubleClicked(QModelIndex index);
    void onPasswordTyped(QString password);
    void onNewRoomButtonClicked();
    void deleteServerRoomSlot();

    void on_connectButt_clicked();

    void on_cancelButt_clicked();

private:
    QMap<QString, qint32> rooms;
    Ui::RoomsListDialog *ui;
    QString localServer;
};

#endif // ROOMSLISTDIALOG_H
