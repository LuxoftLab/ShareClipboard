#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QThread>

#include "clipboardmanager.h"
#include "clientlist.h"
#include "listener.h"
#include "logger.h"

class Launcher : public QObject
{
    Q_OBJECT
public:
    explicit Launcher(QObject *parent = 0);
    ~Launcher();
signals:
public slots:
    void start();
    void exit();

};

#endif // LAUNCHER_H
