#include "tcp_package.h"

QDataStream& operator<<(QDataStream& out, const TcpPackage& pack){
    if(pack.getHeader()->type == RAW || pack.getHeader()->type == INVALID_PASS){
        out << pack.getHeader()->type;
        out << pack.getHeader()->length;
        out << pack.getData()->rawData;
    }
    else if(pack.getHeader()->type == TEXT || pack.getHeader()->type == MEMBER
            || pack.getHeader()->type == PASS || pack.getHeader()->type == ADRESS
            || pack.getHeader()->type == REMOVE){
            out << pack.getHeader()->type;
            out << pack.getHeader()->length;
            out << pack.getData()->strData;
    }
    else
        throw pack.getHeader()->type;

    return out;
}

const QDataStream& operator>>(QDataStream& in, TcpPackage& pack){

    TcpPackageHeader head;
    int temp_type;
    in >> temp_type;
    head.type = (pckg_t)temp_type;
    in >> head.length;
    Data dat;
    dat.rawData = new char[head.length];

    if(head.type == RAW || head.type == INVALID_PASS){
        in >> dat.rawData;
    }
    else if(head.type == TEXT || head.type == MEMBER
            || head.type == PASS || head.type == ADRESS
            || head.type == REMOVE){
        in >> *dat.strData;
    }
    else
        throw head.type;

    pack.setHeader(head);
    pack.setData(dat);
    return in;
}

TcpPackageHeader::TcpPackageHeader(char t, int l){
    type = static_cast<pckg_t>(t);
    length = l;
}

void TcpPackage::setData(Data d){
    this->interior = d;
}

void TcpPackage::setHeader(TcpPackageHeader head){
    this->header = head;
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
