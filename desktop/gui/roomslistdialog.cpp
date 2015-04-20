#include "roomslistdialog.h"

RoomsListDialog::RoomsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomsListDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNewRoomButtonClicked()));
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListItemDoubleClicked(QModelIndex)));
    connect(this, SIGNAL(deleteServerRoom()), this, SLOT(deleteServerRoomSlot()));

    localServer = "";
}

RoomsListDialog::~RoomsListDialog()
{
    delete ui;
}

void RoomsListDialog::onPasswordTyped(QString password)
{
    QString name = ui->listWidget->selectedItems()[0]->text();
    qDebug() << "selected item name: " << name;
    qDebug() << "Selected ip " <<  QHostAddress(rooms[name]);
    emit roomChoosed(rooms[name], password);
    close();
}

void RoomsListDialog::onNewRoomButtonClicked()
{  
    if(localServer ==  NULL) {
        CreateRoomDialog dialog;
        connect(&dialog, SIGNAL(createRoom(QString,QString)),
            this, SIGNAL(newRoomCreated(QString,QString)));
        dialog.exec();
    }
    else {
        emit deleteServerRoom();
    }
}

void RoomsListDialog::deleteServerRoomSlot()
{
    emit deleteRoom(localServer);
}

void RoomsListDialog::onServerIsUp(QString serverName)
{
    localServer = serverName;
    ui->pushButton->setText("Delete my room");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(deleteServerRoomSlot()));
}

void RoomsListDialog::onListItemDoubleClicked(QModelIndex index)
{
    accept();
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

void RoomsListDialog::deleteRoom(QString name)
{
    if(name == localServer) {
        localServer = "";
        ui->pushButton->setText("New");
    }
    qDebug() << name;
    rooms.remove(name);

    QList <QListWidgetItem*> list = ui->listWidget->findItems(name, Qt::MatchFixedString);
    for (QList<QListWidgetItem*>::iterator it = list.begin(); it < list.end(); ++it)
        qDebug() << (*it)->text();

    qDebug() << list.empty();
    int row = ui->listWidget->row(*list.begin());
    QListWidgetItem* item = ui->listWidget->takeItem(row);
    ui->listWidget->removeItemWidget(item);
}
