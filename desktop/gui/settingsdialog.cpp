#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "QDebug"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
//    qDebug() << "accept";
//    if(ui->radioButton->isChecked()) {
//        emit settingsAccepted(true, ui->spinBox->value());
//    } else {
//        emit settingsAccepted(false, ui->spinBox_2->value());
//    }
    this->close();
}

void SettingsDialog::on_radioButton_3_clicked()
{
    ui->spinBox_2->setEnabled(true);
    ui->spinBox->setDisabled(true);
}

void SettingsDialog::on_radioButton_clicked()
{
    ui->spinBox->setEnabled(true);
    ui->spinBox_2->setDisabled(true);
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->close();
}
