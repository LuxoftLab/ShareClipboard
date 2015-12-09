#include "loginpassdialog.h"
#include "ui_loginpassdialog.h"

LoginPassDialog::LoginPassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPassDialog)
{
    ui->setupUi(this);
}

LoginPassDialog::~LoginPassDialog()
{
    delete ui;
}

void LoginPassDialog::on_buttonBox_rejected()
{
    this->close();
}

void LoginPassDialog::on_buttonBox_accepted()
{
    emit credentials(ui->lineEdit->text(), ui->lineEdit_2->text());
}
