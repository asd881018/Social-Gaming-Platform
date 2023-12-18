#ifndef RUNNING_GAME_H
#define RUNNING_GAME_H

#include "GameRoom.h"
#include "Node.h"
#include "DataBucket.h"
#include "GameInfo.h"
#include "GameScanner.h"
#include "GameRunner.h"
#include "GameParams.h"

namespace roomState {

class RunningGame : public RoomState {
public:
    explicit RunningGame(const GameParams& params);

    RoomDeque poll(const jsonMsg& roomMessage, GameRoom& room) override;

private:
    bool gameComplete(DummyBucket bucket);

    GameInfo gameInfo;

    const std::vector<GameInfo>& catalogue;

    Node instruction;

    DataBucket bucket;

    // todo: update to use actual DataBucket
    std::deque<networking::Message> createMessagesFromBucket(const DummyBucket& bucket);

    std::unordered_map<uintptr_t ,std::string> response;

    GameRunner gameRunner;
};

}

#endif