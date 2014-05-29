#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QTimer>

#include "logger.h"

namespace Ui {
class LogView;
}

class LogView : public QWidget
{
    Q_OBJECT

public:
    LogView(QWidget *parent);
    ~LogView();

private slots:
    void onTimerTimeout();
private:
    Ui::LogView *ui;
    QTextBrowser * m_log;
    QTimer m_timer;
};

#endif // LOGVIEW_H
