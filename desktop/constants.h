#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QHostAddress>
#include <QDateTime>
#include <memory>

#define PORT_NUMBER 1080
#define REPEAT 10
#define UDP_PORT 12345
#define MAX_FILE_SIZE 50 * 1024 * 1024

enum FloatServerPriority
{
    PC,
    MOBILE,
    FLOAT_IDLE
};

struct SharedFile{
    QString name;
    QDateTime timeStamp;
    SharedFile(QString, QDateTime);
    SharedFile();
    bool operator==(const SharedFile &);
    bool operator<(const SharedFile &) const;
};


#endif // CONSTANTS_H
