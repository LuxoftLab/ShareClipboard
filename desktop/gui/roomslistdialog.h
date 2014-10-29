#ifndef ROOMSLISTDIALOG_H
#define ROOMSLISTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class RoomsListDialog;
}

class RoomsListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomsListDialog(QList<QString> rooms, QWidget * parent = 0);
    ~RoomsListDialog();

signals:
    void roomChoosed(QString roomName, QString password);

public slots:
   void accept();

private slots:
    void listItemDoubleClicked();
    void onPasswordTyped(QString password);

private:
    Ui::RoomsListDialog *ui;

};

#endif // ROOMSLISTDIALOG_H
