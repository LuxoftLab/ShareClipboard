#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>

namespace Ui {
class CreateRoomDialog;
}

class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoomDialog(QWidget *parent = 0);
    ~CreateRoomDialog();

signals:
    void createRoom(QString name, QString password);
    void onServerCreated(QString serverName);

public slots:
   void accept();

private:
    Ui::CreateRoomDialog *ui;
};

#endif // CREATEROOMDIALOG_H
