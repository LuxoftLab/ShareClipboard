#ifndef CHANGENAMEDIALOG_H
#define CHANGENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class ChangeNameDialog;
}

class ChangeNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeNameDialog(QWidget *parent = 0);
    ~ChangeNameDialog();

signals:
    void nameChoosed(QString name);

public slots:
   void accept();

private:
    Ui::ChangeNameDialog *ui;
};

#endif // CHANGENAMEDIALOG_H
