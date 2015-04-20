#include "changenamedialog.h"
#include "ui_changenamedialog.h"

ChangeNameDialog::ChangeNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeNameDialog)
{
    ui->setupUi(this);
}

ChangeNameDialog::~ChangeNameDialog()
{
    delete ui;
}

void ChangeNameDialog::accept()
{
    emit nameChoosed(ui->lineEdit->text());
    close();
}
