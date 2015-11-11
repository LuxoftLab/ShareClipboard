#ifndef TCPPACKAGES
#define TCPPACKAGES

enum TcpPackage
{
    // Request packages
     AUTH,

    // Respond packages
    AUTH_SUCC,
    AUTH_FAIL,

    //Data packages
    TXT,
    PNG,
    FIL
};



#endif // TCPPACKAGES

