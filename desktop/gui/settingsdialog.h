#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "constants.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    qint32 defaultMaxSize = MAX_FILE_SIZE;
    QString defaultFilePath = "/tmp/";
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
signals:
    void settingsAccepted(qint32, QString);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();


private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
