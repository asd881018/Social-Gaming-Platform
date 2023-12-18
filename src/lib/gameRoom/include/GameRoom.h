#ifndef GAME_ROOM_H
#define GAME_ROOM_H

#include "Server.h"
#include "RoomState.h"
#include "Setup.h"
#include "RunningGame.h"
#include "msgCreator.h"

namespace roomState {
    class RoomState;
}

class GameRoom {
public:
    GameRoom(networking::Connection owner, GameRoomID roomCode, std::unique_ptr<roomState::RoomState>&& startState);


    RoomDeque poll(const jsonMsg& roomMessages);

    void changeState(std::unique_ptr<roomState::RoomState>&& newState);

    [[nodiscard]] GameRoomID getRoomCode() const;

    [[nodiscard]] const networking::Connection& viewOwner() const;

    [[nodiscard]] const std::vector<networking::Connection>& viewPlayers() const;

private:
    networking::Connection owner;

    // list of clients in the game room
    std::vector<networking::Connection> players;

    std::unique_ptr<roomState::RoomState> state;

    // room code is used to connect clients to the gameRoom
    GameRoomID roomCode;

    std::deque<jsonMsg> checkNewPlayers(const jsonMsg& msg);

    RoomDeque checkQuitters(const jsonMsg& msg);
};

#endif