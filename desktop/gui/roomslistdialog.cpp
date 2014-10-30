#include "roomslistdialog.h"
#include "ui_roomslistdialog.h"
#include "passworddialog.h"

RoomsListDialog::RoomsListDialog(QList<QString> rooms, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomsListDialog)
{
    ui->setupUi(this);
    QList<QString>::iterator i;
    for(i = rooms.begin(); i != rooms.end(); i++) {
        ui->listWidget->addItem(*i);
    }



    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)),
                                   this, SLOT(listItemDoubleClicked(QModelIndex)));
}

RoomsListDialog::~RoomsListDialog()
{
    delete ui;
}

void RoomsListDialog::listItemDoubleClicked() {
    emit accept();
}

void RoomsListDialog::onPasswordTyped(QString password)
{
    emit roomChoosed(ui->listWidget->selectedItems()[0]->text(),
            password);
}

void RoomsListDialog::accept() {
    if(ui->listWidget->selectedItems() == NULL) return;

    PasswordDialog dialog(this);
    connect(&dialog, SIGNAL(passwordTyped(QString)), this, SLOT(onPasswordTyped(QString));
    dialog.exec();


}
