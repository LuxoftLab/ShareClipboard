#ifndef UDP_SERVICE_H
#define UDP_SERVICE_H

#include "controller.h"
class Controller;
class UDPService
{
    Controller * controller;
public:
    UDPService(Controller * controller);
    bool initListener();
    void getRooms();
    void sendRoom(QString name);
    void notifyAboutRoom(QString name);
};

#endif // UDP_SERVICE_H
