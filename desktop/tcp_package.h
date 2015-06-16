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
    FILE,
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
    void encode();
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
};

class MemberPackage : public TcpPackage
{
    void decode(QDataStream &in);
};

class RemoveMemberPackage : public TcpPackage
{
    void decode(QDataStream &in);
};

class ImagePackage : public TcpPackage
{
    void decode(QDataStream &in);
};

class PassPackage : public TcpPackage
{
    void decode(QDataStream &in);
};

class TcpPackageFactory
{
public:
    TcpPackage * getPackage(pckg_t type);
};

#endif // TCP_PACKAGE_H
