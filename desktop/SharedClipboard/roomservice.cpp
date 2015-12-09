#include "roomservice.h"


QList<RoomMember> RoomService::getRoomMembers() const
{
    return roomMembers;
}
RoomService::RoomService(QObject *parent) : QObject(parent)
{
    checkAlivesTimer = new QTimer(this);
    connect(checkAlivesTimer, &QTimer::timeout, this, &RoomService::checkAlives);
    checkAlivesTimer->start(CHECK_ALIVE_TIMEOUT_MSEC);
}

QList<QString> RoomService::getRooms()
{
    return roomsNames;
}

void RoomService::addMember(QString l, QString r, QList<QHostAddress> addrl)
{
    // add roomname to roomslist, if not already in it, add member if it is in our room list and is not yet there
        if(!roomsNames.contains(r)){
            roomsNames.append(r);
        }

        if(r != ownRoomName)
            return;
        RoomMember candidate = RoomMember(l, addrl, true, 0);
        if(roomMembers.contains(candidate)){
            int candidate_index = roomMembers.lastIndexOf(candidate);
            RoomMember present = roomMembers.at(candidate_index);
            roomMembers.removeAt(candidate_index);
            present.alive = true;
            if(present.packs_count < 200){
                present.packs_count++;
            }
            roomMembers.append(present);
        } else {
            roomMembers.append(RoomMember(l, addrl, true, 0));
        }
}

void RoomService::checkAlives()
{
    for (auto member : roomMembers){
        if(member.packs_count < MIN_RESP_COUNT){
            member.packs_count = 0;
            member.alive = false;
        }
    }
    emit refreshMembers(this->roomMembers);
}

void RoomService::setRoom(QString & value)
{
    this->ownRoomName = value;
}



