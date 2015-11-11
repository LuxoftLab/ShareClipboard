#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QDataStream>

class SessionManager
{
    SessionManager();
public:
    static QString sharedClipboardIdFileName;
    static QString idFilePath;

    static bool sessionActive();
    static bool sessionInterrupted();

    static bool idFileExists();
    static bool idFileOpenedForWriting();

    static QString getLogin(QFile source);
    static QString setLogin(QString login);
};



#endif // SESSIONMANAGER_H
