#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QString roomName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
    ui->label_2->setText("Комната " + roomName);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::accept()
{
    emit passwordTyped(ui->lineEdit->text());
    this->close();
}
