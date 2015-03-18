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

class PackageHandler : public QObject
{
    Q_OBJECT
protected:
    qint32 address;
    int size;
    char* text;
    char* login;
    char* image;
public:
    virtual void decode(QDataStream& in) = 0;
    void encode();
};

class ServerConnectionHandler : public PackageHandler
{
    Q_OBJECT
signals:
    void gotText(QString);
    void gotImage(QByteArray);
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

class ClientConnectionHandler : public PackageHandler{};

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
