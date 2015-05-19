#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QImage>

//#include "client_connection.h"
//class ClientConnection;

#ifndef TCP_PACKAGE_H
#define TCP_PACKAGE_H

enum pckg_t
{
    TEXT,
    IMAGE,
    PASS,
    MEMBER,
    INVALID_PASS,
    REMOVE
};

//PackageHandler
class PackageHandler : public QObject
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
};

//ServerCOnnectionHandler
class ServerConnectionHandler : public PackageHandler
{
    Q_OBJECT
signals:
    void gotText(QString);
    void gotImage(QByteArray);
    void gotData(QByteArray, QString);
    void addMember(QString, QHostAddress);
    void deleteMember(QHostAddress);
};

class ServerConnectionHandlerText : public ServerConnectionHandler
{
    Q_OBJECT
public:
    void decode(QDataStream &in);
};

class ServerConnectionHandlerRemoveMember : public ServerConnectionHandler
{
    Q_OBJECT
public:
    void decode(QDataStream &in);
};

class ServerConnectionHandlerMember : public ServerConnectionHandler
{
    Q_OBJECT
public:
    void decode(QDataStream &in);
};

class ServerConnectionHandlerImage : public ServerConnectionHandler
{
    Q_OBJECT
public:
    void decode(QDataStream &in);
};

//ClientConnection Handler
class ClientConnectionHandler : public PackageHandler
{
    Q_OBJECT
public:
    //void decode(QDataStream&);
signals:
    //void onText(QString, ClientConnection * const);
    //void onImage(QByteArray);
    //void verifyPass(QString, ClientConnection * const);
};


//class ClientConnectionHandlerText : public ClientConnectionHandler
//{
//    Q_OBJECT
//public:
//    void decode(QDataStream &in);
//};


//class ClientConnectionHandlerVerifyPass : public ClientConnectionHandler
//{
//    Q_OBJECT
//public:
//    //void decode(QDataStream &in, ClientConnection * const);
//};

//class ClientConnectionHandlerImage : public ClientConnectionHandler
//{
//    Q_OBJECT
//public:
//    void decode(QDataStream &in);
//};

//Factory
class Factory
{
public:
    PackageHandler* getHandler(pckg_t type);
};

class ServerConnectionFactory : public Factory
{
public:
    ServerConnectionHandler *getHandler(pckg_t type);
};

class ClientConnectionFactory : public Factory
{
public:
   ClientConnectionHandler* getHandler(pckg_t type);
};

#endif // TCP_PACKAGE_H
