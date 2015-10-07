#include <sharedPackages/package.h>

//TcpPackage *TcpPackageFactory::getPackage(pckg_t type)
//{
//    switch(type){
//    case MEMBER:
//        return new MemberPackage();
//    case TEXT:
//        return new DataPackage();
//    case REMOVE:
//        return new RemoveMemberPackage();
//    case IMAGE:
//        return new ImagePackage();
//    case FILENOTIF:
//        return new FileNotificationPackage();
//    case FILEREQ:
//        return new FileReqPackage();
//    case FILERESP:
//        return new FileRespPackage();
//    case PASS:
//        return new PassPackage();
//      case INVALID_PASS:
//          break;
//   default: throw type;
//    }
//}


TcpPackage::~TcpPackage()
{

}

TcpPackage::TcpPackage()
{

}
