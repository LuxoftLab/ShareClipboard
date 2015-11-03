#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QObject>
#include "common.h"

class FileService : public QObject
{
    Q_OBJECT
public:
    explicit FileService(QObject *parent = 0);

    QByteArray fileDataToByteArray(QString);
    SharedFile byteArrayToFileData(QByteArray&);
    QByteArray getFileChunk(qint32 start, size_t size, QString fullpath);
signals:

public slots:
};

#endif // FILESERVICE_H
