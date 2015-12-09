#ifndef LOGINPASSDIALOG_H
#define LOGINPASSDIALOG_H

#include <QDialog>

namespace Ui {
class LoginPassDialog;
}

class LoginPassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginPassDialog(QWidget *parent = 0);
    ~LoginPassDialog();

signals:
    void credentials(QString, QString);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::LoginPassDialog *ui;
};

#endif // LOGINPASSDIALOG_H
