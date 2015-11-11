#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>
#include <QTemporaryDir>

#include "control.h"
#include "sessionmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

   QSharedPointer<Control> controller;
   if(SessionManager::sessionActive()){
       return 0;
   } else if(SessionManager::sessionInterrupted()){
       return app.exec();
   }else{
       QString testLogin = "testlogin_adsfhallsfj;a";
       controller = QSharedPointer<Control>(new Control(testLogin, NULL));
       return app.exec();
   }
}
