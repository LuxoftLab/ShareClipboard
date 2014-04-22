#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QHostAddress>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

class TimeStamp
{
public:
    TimeStamp();
    operator QString();
};

class Logger
{
private:
    Logger();
    Logger& operator=(const Logger& right);
    Logger(const Logger& right);

    static Logger * m_self;
    static QMutex m_guard;

    QFile m_log_file;
    QTextStream m_stream;
public:

    class LoggerIOException{};

    static Logger & instance();
    Logger & operator <<(const QString & str);
    Logger & operator <<(const char * str);
    Logger & operator <<(const QHostAddress & addr);
    Logger & operator <<(const int & num);
};

#endif // LOGGER_H
