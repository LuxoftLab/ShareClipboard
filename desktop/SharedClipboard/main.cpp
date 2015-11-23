#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>
#include <QTemporaryDir>

#include "sessionmanager.h"

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
   QSharedPointer<EncryptionService> encService;
   QSharedPointer<SessionManager> sessionManager;

   if(SessionManager::sessionActive()){
       return 0;
   } else if(SessionManager::sessionInterrupted()){
       return app.exec();
   }else{
        sessionManager = QSharedPointer<SessionManager>(SessionManager::getInstance());
        sessionManager->startSession(roomService,
                                     udpService,
                                     tcpService,
                                     clipboardService,
                                     fileService,
                                     encService);
       return app.exec();
   }
}
