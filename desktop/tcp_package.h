#ifndef TCP_PACKAGE_H
#define TCP_PACKAGE_H

#include <QDataStream>

enum pckg_t{TEXT, PASS, LOGIN, RAW};

union Data{
    QString* strData;
    QString* pass;
    char* rawData;
};

struct TcpPackageHeader{

    pckg_t type;
    int length;
    TcpPackageHeader(char t, int l);
    TcpPackageHeader();
};

class TcpPackage{
    friend QDataStream& operator <<(QDataStream&, const TcpPackage&);
    friend const QDataStream& operator >>(const QDataStream&, TcpPackage&);
public:
    TcpPackage(TcpPackageHeader, Data);
    TcpPackage();
    const TcpPackageHeader* getHeader() const;
    const Data* getData() const;

private:
    TcpPackageHeader header;
    Data interior;

};


#endif // TCP_PACKAGE_H
