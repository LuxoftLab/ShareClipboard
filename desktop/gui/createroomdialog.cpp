#include "createroomdialog.h"
#include "ui_createroomdialog.h"
#include "mainwindow.h"
#include <QMessageBox>

CreateRoomDialog::CreateRoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateRoomDialog)
{
    ui->setupUi(this);
}

CreateRoomDialog::~CreateRoomDialog()
{
    delete ui;
}

void CreateRoomDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox messageBox;
        messageBox.setText(tr("Введите имя комнаты!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    if(ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox messageBox;
        messageBox.setText(tr("Введите пароль!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    if(ui->lineEdit_2->text().compare(ui->lineEdit_3->text()) != 0)
    {
        QMessageBox messageBox;
        messageBox.setText(tr("Пароли не совпадают"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    emit createRoom(ui->lineEdit->text(), ui->lineEdit_2->text());
    close();
}

void CreateRoomDialog::on_buttonBox_accepted()
{
    this->accept();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit->setFocus();
}
