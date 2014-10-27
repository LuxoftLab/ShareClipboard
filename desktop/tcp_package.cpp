#include "tcp_package.h"

QDataStream& operator<<(QDataStream& out, const TcpPackage&){
    return out;
}

const QDataStream& operator>>(const QDataStream& in, TcpPackage&){
    return in;
}

TcpPackageHeader::TcpPackageHeader(char t, int l){
    type = static_cast<pckg_t>(t);
    length = l;
}

TcpPackageHeader::TcpPackageHeader(){}

TcpPackage::TcpPackage(TcpPackageHeader header, Data data){
    this->header = header;
    this->interior = data;
}

TcpPackage::TcpPackage(){}


const TcpPackageHeader* TcpPackage::getHeader() const{
    return &header;
}

const Data* TcpPackage::getData() const{
    return &interior;
}
