#include "roomchoose.h"
#include "ui_roomchoose.h"

RoomChoose::RoomChoose(QList<QString>, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomChoose)
{
    ui->setupUi(this);
    //ui->listView->setModel();
}

RoomChoose::~RoomChoose()
{
    delete ui;
}

void RoomChoose::on_buttonBox_rejected()
{
    this->close();
}

void RoomChoose::on_buttonBox_accepted()
{
    emit qvariant_cast<QString>(ui->listView->model()->data(ui->listView->currentIndex()));
}
