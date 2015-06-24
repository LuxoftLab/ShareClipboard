#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QImage>
#include <QTcpSocket>
#include <assert.h>
#include <exception>

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
    virtual void write(QTcpSocket *) = 0;
    virtual void read(QDataStream &) = 0;
signals:
    void gotText(QString);
    void gotImage(QByteArray);
    void gotPass(QString, floating_server_priorities);
    void gotData(QByteArray, QString);
    void addMember(QString, floating_server_priorities, QHostAddress);
    void deleteMember(QHostAddress);
};

class DataPackage : public TcpPackage
{
    QByteArray data;
    pckg_t type;
public:
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class MemberPackage : public TcpPackage
{
public:
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class RemoveMemberPackage : public TcpPackage
{
public:
    void write(QTcpSocket *);
    void read(QDataStream &);
};

class ImagePackage : public TcpPackage
{
public:
    ImagePackage();
    void write(QTcpSocket *);
    void read(QDataStream &);
};

class PassPackage : public TcpPackage
{
    floating_server_priorities priority;
    QString password;
public:
    PassPackage(QString, floating_server_priorities);
    PassPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class TcpPackageFactory
{
public:
    TcpPackage * getPackage(pckg_t type);
};

#endif // TCP_PACKAGE_H
