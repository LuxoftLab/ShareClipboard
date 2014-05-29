#include "logger.h"

Logger * Logger::m_self=0;
QMutex Logger::m_guard;

Logger::Logger()
      : m_log_file(QString("log_%1.log").arg(QDateTime::currentMSecsSinceEpoch()))
{
    if(!m_log_file.open(QIODevice::ReadWrite))
        throw LoggerIOException();
    m_stream.setDevice(&m_log_file);
    m_buffer.open(QIODevice::WriteOnly);
    m_buffer_stream.setDevice(&m_buffer);
}

Logger &Logger::operator=(const Logger &right)
{
    return *this;
}

Logger::Logger(const Logger &right)
{

}

Logger &Logger::instance()
{
    QMutexLocker lock(&m_guard);
    if(m_self==0)
        m_self=new Logger();
    return *m_self;
}

Logger &Logger::operator <<(const QString &str)
{
    QMutexLocker lock(&m_guard);
    m_feeds[QThread::currentThread()]<<str;
    return *this;
}

Logger &Logger::operator <<(const char *str)
{
    QMutexLocker lock(&m_guard);
    m_feeds[QThread::currentThread()]<<str;
    return *this;
}

Logger &Logger::operator <<(const QHostAddress &addr)
{
    QMutexLocker lock(&m_guard);
    m_feeds[QThread::currentThread()]<<addr.toString();
    return *this;
}

Logger &Logger::operator <<(const int &num)
{
    QMutexLocker lock(&m_guard);
    m_feeds[QThread::currentThread()]<<QString::number(num);
    return *this;
}

Logger &Logger::operator <<(const Flush &f)
{
    QMutexLocker lock(&m_guard);
    QQueue<QString> & q=m_feeds[QThread::currentThread()];
    QString tmp;
    while(!q.isEmpty())
    {
        tmp=q.dequeue();
        m_stream<<tmp;
        m_buffer_stream<<tmp;
    }
    m_stream<<endl;
    m_buffer_stream<<endl;
    m_stream.flush();
    m_buffer_stream.flush();
    return *this;
}

QByteArray Logger::readBuffer()
{
    QMutexLocker lock(&m_guard);
    QByteArray ba=m_buffer.data();
    m_buffer.seek(0);
    m_buffer.buffer().clear();
    return ba;
}

TimeStamp::TimeStamp()
{
}

TimeStamp::operator QString()
{
    return QString("[%1] ").arg(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss"));
}
