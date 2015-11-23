#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>
#include <QTemporaryDir>

#include "roomservice.h"
#include "sessionmanager.h"
#include "clipboardservice.h"
#include "fileservice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

   QSharedPointer<RoomService> roomService;
   QSharedPointer<UdpService> udpService;
   QSharedPointer<TcpService> tcpService;
   QSharedPointer<ClipboardService> clipboardService;
   QSharedPointer<FileService> fileService;

   if(SessionManager::sessionActive()){
       return 0;
   } else if(SessionManager::sessionInterrupted()){
       return app.exec();
   }else{ // start session
       QString testLogin = "testlogin_adsfhallsfj;a";
       roomService = QSharedPointer<RoomService>(new RoomService(testLogin, NULL));
       QList<QString> room_list = roomService->getRooms();
       QString room = "room test name";
       roomService->setRoom(room);

       udpService = QSharedPointer<UdpService>(new UdpService(testLogin, NULL));
       udpService->setRoomName(room);
       QObject::connect(udpService.data(), &UdpService::newMember,
            roomService.data(), &RoomService::addMember);

       // start sniffing for other members over udp
       udpService->start();

       tcpService = QSharedPointer<TcpService>(new TcpService());
       tcpService->createServer();

       return app.exec();
   }
}
