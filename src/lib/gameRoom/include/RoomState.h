#ifndef ROOM_STATE_H
#define ROOM_STATE_H

#include "GameRoom.h"
#include "msgCreator.h"
#include "RoomDeque.h"

class GameRoom;

namespace roomState {

class RoomState {
public:
    virtual RoomDeque
    poll(const jsonMsg& roomMessages, GameRoom& room) = 0;

    virtual ~RoomState() = default;
};

}

#endif