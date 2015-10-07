#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QImage>
#include <QTcpSocket>
#include <assert.h>
#include <QDateTime>

#include "constants.h"

enum pckg_t
{
    TCP_IDLE,
    TEXT,
    IMAGE,
    FILENOTIF,
    FILEREQ,
    FILERESP,
    PASS,
    MEMBER,
    INVALID_PASS,
    REMOVE
};

const int PCKG_SZ_FIELD_SZ = sizeof(qint32);
const int PCKG_TYPE_FIELD_SZ = sizeof(pckg_t);
const int BYTE_ARR_SZ_FIELD_SZ = sizeof(qint32);

class TcpPackage : public QObject
{
    Q_OBJECT
protected:
    qint32 address;
    int size;
    int size2;
    char* text;
    char* login;
    char* image;
    char* password;
public:
    virtual void write(QTcpSocket *) = 0;
    virtual void read(QDataStream &) = 0;
    virtual ~TcpPackage();
    TcpPackage();
signals:
    void gotText(QString);
    void gotImage(QByteArray);
    void gotPass(QString, FloatServerPriority);
    void gotData(QByteArray, QString);
    void gotFileNotification(QString, QDateTime);
    void addMember(FloatServerPriority, QHostAddress);
    void deleteMember(QHostAddress);
    void gotFileReq(QString, QDateTime);
    void gotFileResp(QString, QDateTime, QByteArray);
};


#endif
