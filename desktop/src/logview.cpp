#include "logview.h"
#include "ui_logview.h"

LogView::LogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogView)
{
    ui->setupUi(this);
    m_log=new QTextBrowser();
    QVBoxLayout * layout=new QVBoxLayout();
    layout->addWidget(m_log);
    this->setLayout(layout);
    m_timer.setInterval(500);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_timer.start();
    setWindowTitle(tr("Log viewer"));
    connect(&m_timer,&QTimer::timeout,this,&LogView::onTimerTimeout);
}

LogView::~LogView()
{
    delete ui;
}

void LogView::onTimerTimeout()
{
    QByteArray buf=Logger::instance().readBuffer();
    if(buf.length())
        m_log->append(QString(buf).toLocal8Bit());
}
