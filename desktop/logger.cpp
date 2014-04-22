#include "logger.h"

Logger * Logger::m_self=0;
QMutex Logger::m_guard;

Logger::Logger()
    : m_log_file(QString("log_%1.log").arg(QDateTime::currentMSecsSinceEpoch()))
{
    if(!m_log_file.open(QIODevice::ReadWrite))
        throw LoggerIOException();
    m_stream.setDevice(&m_log_file);
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
    m_stream<<str;
    //m_log_file.flush();
    m_stream.flush();
    return *this;
}

Logger &Logger::operator <<(const char *str)
{
    QMutexLocker lock(&m_guard);
    m_stream<<str;
    m_stream.flush();
    return *this;
}

Logger &Logger::operator <<(const QHostAddress &addr)
{
    QMutexLocker lock(&m_guard);
    m_stream<<addr.toString();
    m_stream.flush();
    return *this;
}

Logger &Logger::operator <<(const int &num)
{
    QMutexLocker lock(&m_guard);
    m_stream<<num;
    m_stream.flush();
    return *this;
}

TimeStamp::TimeStamp()
{
}

TimeStamp::operator QString()
{
    return QString("[%1] ").arg(QDateTime::currentDateTime().toString());
}
