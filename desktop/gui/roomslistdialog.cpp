#include "roomslistdialog.h"
#include "ui_roomslistdialog.h"
#include "passworddialog.h"
#include "createroomdialog.h"

RoomsListDialog::RoomsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomsListDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNewRoomButtonClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onDelRoomButtonClicked()));
    ui->pushButton_2->setHidden(1);
}

RoomsListDialog::~RoomsListDialog()
{
    delete ui;
}

void RoomsListDialog::setRoomsHash(QHash<QString, qint32> rooms)
{
    QList<QString> roomsList = rooms.keys();
    QList<QString>::iterator i;
    for(i = roomsList.begin(); i != roomsList.end(); ++i)
    {
        ui->listWidget->addItem(*i);
    }
}

void RoomsListDialog::onPasswordTyped(QString password)
{
    QString name = ui->listWidget->selectedItems()[0]->text();
    emit roomChoosed(rooms[name], password);
    close();
}

void RoomsListDialog::onNewRoomButtonClicked()
{
    CreateRoomDialog dialog;
    connect(&dialog, SIGNAL(createRoom(QString,QString)),
            this, SIGNAL(newRoomCreated(QString,QString)));
    dialog.exec();
}

void RoomsListDialog::onDelRoomButtonClicked()
{
    //if(ui->listWidget->selectedItems().isEmpty()) return;

    PasswordDialog dialog(ui->listWidget->selectedItems()[0]->text(), this);
    //connect(&dialog, SIGNAL(passwordTyped(QString)), this, SLOT(onPasswordTyped(QString)));
    dialog.exec();
}

void RoomsListDialog::accept()
{
    if(ui->listWidget->selectedItems().isEmpty()) return;

    PasswordDialog dialog(ui->listWidget->selectedItems()[0]->text(), this);
    connect(&dialog, SIGNAL(passwordTyped(QString)), this, SLOT(onPasswordTyped(QString)));
    dialog.exec();
}

void RoomsListDialog::addRoom(QString name, qint32 id)
{
    rooms.insert(name, id);
    ui->listWidget->addItem(name);

}
/*
void RoomsListDialog::deleteRoom(QString name)
{
    //looks awful but it works
    rooms.remove(name);
    QList<QListWidgetItem*> list = ui->listWidget->findItems(name, Qt::MatchFixedString);
    int row = ui->listWidget->row(list.first());
    QListWidgetItem* item = ui->listWidget->takeItem(row);
    delete item;
}
*/
