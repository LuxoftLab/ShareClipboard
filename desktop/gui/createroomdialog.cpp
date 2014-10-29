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
    emit ((MainWindow)parent()).roomCreated(
                ui->lineEdit->text(),
                ui->lineEdit_2->text(),
                "SOME LOGIN");  // TODO: Спроить, что за логин и где он хранится
}
