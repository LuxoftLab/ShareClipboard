#include "sessionmanager.h"

QString SessionManager::sharedClipboardIdFileName = QString::fromUtf8(".SharedClipboard/.config");
QString SessionManager::idFilePath = QDir::homePath() + "/" + sharedClipboardIdFileName;

SessionManager::SessionManager(){}


bool SessionManager::idFileExists()
{
    QFile idFile(idFilePath);
    if(idFile.exists()){
        return true;
    } else {
        return false;
    }
}

bool SessionManager::idFileOpenedForWriting()
{
    QFile file(idFilePath);
    bool ready = file.open(QIODevice::WriteOnly);
    file.close();
    return !ready;
}


QString SessionManager::getLogin(QFile source)
{
    if(idFileExists() && source.open(QIODevice::ReadOnly)){
            QDataStream idFileStream(&source);
            QString login;
            idFileStream >> login;
            return login;
    }
    return nullptr;
}

bool SessionManager::sessionActive()
{
    if(idFileExists() && idFileOpenedForWriting())
        return true;
    return false;
}

bool SessionManager::sessionInterrupted()
{
    if(idFileExists() && !idFileOpenedForWriting())
        return true;
    return false;
}


void checkLogin()
{

}


