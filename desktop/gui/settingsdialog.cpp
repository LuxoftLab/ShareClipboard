#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "QDebug"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->maxfilesizespinBox->setValue(defaultMaxSize/(1024*1024)); // in MB
    ui->filePath->setText(defaultFilePath);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    defaultMaxSize = ui->maxfilesizespinBox->value() * 1024 * 1024;
    defaultFilePath = ui->filePath->text();

    emit settingsAccepted(ui->maxfilesizespinBox->value(), ui->filePath->text());
    this->close();
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->close();
}
