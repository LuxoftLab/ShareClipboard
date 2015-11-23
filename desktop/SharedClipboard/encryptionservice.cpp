#include "encryptionservice.h"

EncryptionService::EncryptionService(QString & value)
{
    key = value;
}

QByteArray EncryptionService::encode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    return data;
}

QByteArray EncryptionService::decode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    return data;
}
