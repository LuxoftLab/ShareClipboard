#include <QApplication>
#include <QQmlApplicationEngine>

#include "control.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    Control control("testLogin");
    return app.exec();
}
