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
    SHAREDFILE,
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
    void gotFileNotification(QString, QHostAddress);
    void addMember(QString, floating_server_priorities, QHostAddress);
    void deleteMember(QHostAddress);
};

class FailPackage : public TcpPackage
{
public:
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class DataPackage : public TcpPackage
{
    QByteArray data;
    pckg_t type;
public:
    DataPackage(QByteArray&, pckg_t);
    DataPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class FileNotificationPackage : public TcpPackage
{
    QByteArray data;
    QHostAddress sourceAddress;

public:
    FileNotificationPackage();
    FileNotificationPackage(QHostAddress, QByteArray&);

    void read(QDataStream &);
    void write(QTcpSocket *);
};

class MemberPackage : public TcpPackage
{
    QString login;
    QHostAddress addr;
    floating_server_priorities prior;
public:
    MemberPackage();
    MemberPackage(QString, QHostAddress, floating_server_priorities);
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class RemoveMemberPackage : public TcpPackage
{
    QHostAddress removeAddr;
public:
    RemoveMemberPackage();
    RemoveMemberPackage(QHostAddress);
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
