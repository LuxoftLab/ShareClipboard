#include "encryptionservice.h"

EncryptionService::EncryptionService(QString & value)
{
    key = value;
}

void EncryptionService::encode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    emit encoded(data);
}

void EncryptionService::decode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    emit decoded(data);
    //return data;
}
