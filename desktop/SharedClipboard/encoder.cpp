#include "encoder.h"

Encoder::Encoder(QString & value)
{
    key = value;
}

QByteArray Encoder::encode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    return data;
}

QByteArray Encoder::decode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    return data;
}
