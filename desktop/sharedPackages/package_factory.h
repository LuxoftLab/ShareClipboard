#ifndef PACKAGE_DISPATCH_H
#define PACKAGE_DISPATCH_H

#include "datapackage.h"
#include "failpackage.h"
#include "file_notification_package.h"
#include "file_request_package.h"
#include "file_response_package.h"
#include "image_package.h"
#include "memberpackage.h"
#include "password_package.h"
#include "removemember.h"

class TcpPackageFactory
{
public:
    TcpPackage * getPackage(pckg_t type);
};
#endif // PACKAGE_DISPATCH_H
