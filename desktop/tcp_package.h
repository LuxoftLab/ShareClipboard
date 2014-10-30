#ifndef TCP_PACKAGE_H
#define TCP_PACKAGE_H

#include <QDataStream>

enum pckg_t{TEXT, PASS, MEMBER, RAW, INVALID_PASS, ADRESS, REMOVE};

union Data{
    QString* strData;
    char* rawData;
};


//template<typename T>
//const QDataStream& operator>>(const QDataStream& in, T& t){
//    int i;
//    in >> i;
//    t = (pckg_t)i;
//    return in;
//}

struct TcpPackageHeader{

    pckg_t type;
    int length;
    TcpPackageHeader(char t, int l);
    TcpPackageHeader();
};

class TcpPackage{
    friend QDataStream& operator <<(QDataStream&, const TcpPackage&);
    friend const QDataStream& operator >>(QDataStream&, TcpPackage&);
public:
    TcpPackage(TcpPackageHeader, Data);
    TcpPackage();
    const TcpPackageHeader* getHeader() const;
    const Data* getData() const;

    void setHeader(TcpPackageHeader);
    void setData(Data);

private:
    TcpPackageHeader header;
    Data interior;

};


#endif // TCP_PACKAGE_H
