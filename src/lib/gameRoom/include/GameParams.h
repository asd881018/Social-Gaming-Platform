#pragma once

#include <vector>
#include "GameInfo.h"
#include "Server.h"
#include "DataBucket.h"
#include "msgCreator.h"
#include "Expression.h"

struct GameParams {
    GameInfo game;
    std::vector<GameInfo> gameInfos; // todo: group question - what is the use of this?
    std::vector<networking::Connection> players;
    GameRoomID gameRoomId; // todo: group question - best way to store this?  necessary, or other possibility?
//    DataBucket bucket;
    ValueMap globalState;
};

