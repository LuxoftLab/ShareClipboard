#include "createroomdialog.h"
#include "ui_createroomdialog.h"
#include "mainwindow.h"

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
    emit (createRoom(ui->lineEdit->text(), ui->lineEdit_2->text()));
    close();
}
