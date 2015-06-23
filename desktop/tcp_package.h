#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QImage>
#include <assert.h>

#include "constants.h"

#ifndef TCP_PACKAGE_H
#define TCP_PACKAGE_H

enum pckg_t
{
    TEXT,
    IMAGE,
    FILENOTIF,
    PASS,
    MEMBER,
    INVALID_PASS,
    REMOVE
};

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
    virtual void decode(QDataStream& in) = 0;
    virtual QByteArray * encode(QString, QString, floating_server_priorities) = 0;
signals:
    void gotText(QString);
    void gotImage(QByteArray);
    void gotPass(QString, floating_server_priorities);
    void gotData(QByteArray, QString);
    void addMember(QString, floating_server_priorities, QHostAddress);
    void deleteMember(QHostAddress);
};

class TextPackage : public TcpPackage
{
    void decode(QDataStream &in);
    QByteArray * encode(QString, QString, floating_server_priorities);
};

class MemberPackage : public TcpPackage
{
    void decode(QDataStream &in);
    QByteArray * encode(QString, QString, floating_server_priorities);
};

class RemoveMemberPackage : public TcpPackage
{
    void decode(QDataStream &in);
    QByteArray * encode(QString, QString, floating_server_priorities);
};

class ImagePackage : public TcpPackage
{
    void decode(QDataStream &in);
    QByteArray * encode(QString, QString, floating_server_priorities);
};

class PassPackage : public TcpPackage
{
    void decode(QDataStream &in);
    QByteArray * encode(QString, QString, floating_server_priorities);
};

class TcpPackageFactory
{
public:
    TcpPackage * getPackage(pckg_t type);
};

#endif // TCP_PACKAGE_H
