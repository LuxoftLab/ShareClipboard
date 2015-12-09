#ifndef ROOMCHOOSE_H
#define ROOMCHOOSE_H

#include <QDialog>
#include <QList>

namespace Ui {
class RoomChoose;
}

class RoomChoose : public QDialog
{
    Q_OBJECT

public:
    explicit RoomChoose(QList<QString>, QWidget *parent = 0);
    ~RoomChoose();
signals:
    void room(QString);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::RoomChoose *ui;
};

#endif // ROOMCHOOSE_H
