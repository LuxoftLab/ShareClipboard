#include "room.h"

Room::Room(QString name, QString pass)
{
    this->name = name;
    this->pass = pass;
}

QString Room::getName()
{
    return name;
}
