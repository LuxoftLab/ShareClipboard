#include "roomslistdialog.h"
#include "ui_roomslistdialog.h"

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

void RoomsListDialog::accept() {
    ui->listWidget->selectedItems()[0]->text();
}
