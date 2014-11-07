#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QString roomName, QWidget *parent = 0);
    ~PasswordDialog();

signals:
   void passwordTyped(QString password);

public slots:
   void accept();

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H