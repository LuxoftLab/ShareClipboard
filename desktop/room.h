#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QString>

class Room : public QObject
{
    Q_OBJECT
private:
    QString name;
    QString pass;
public:
    Room(QString name, QString pass);
};

#endif // ROOM_H
