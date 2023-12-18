#include "GameRoom.h"
#include "msgCreator.h"
#include "StringResources.h"
#include <string>
#include <vector>
#include <utility>

using networking::Connection;

GameRoom::GameRoom(networking::Connection owner, GameRoomID roomCode,
                   std::unique_ptr<roomState::RoomState>&& startState) :
        owner(owner), players({owner}), state(std::move(startState)), roomCode(roomCode) {
}


RoomDeque GameRoom::poll(const jsonMsg& roomMessage) {
    RoomDeque returnMessages;

    returnMessages.insert(checkNewPlayers(roomMessage));

    returnMessages.insert(checkQuitters(roomMessage));
    returnMessages.insert(state->poll(roomMessage, *this));

    return returnMessages;
}

void GameRoom::changeState(std::unique_ptr<roomState::RoomState>&& newState) {
    this->state = std::move(newState);
}

GameRoomID GameRoom::getRoomCode() const {
    return roomCode;
}

const networking::Connection& GameRoom::viewOwner() const {
    return owner;
}

const std::vector<Connection>& GameRoom::viewPlayers() const {
    return players;
}

std::deque<jsonMsg> GameRoom::checkNewPlayers(const jsonMsg& msg) {
    auto returnMessages = std::deque<jsonMsg>();
    if (std::find_if(players.begin(), players.end(),
                     [&msg](const networking::Connection& connection) { return connection == msg.connection; }) ==
        players.end()) {
        jsonMsg welcomeMessage{.messageContent = "Welcome to the game!", .gameRoom = this->roomCode, .connection = msg.connection};

        returnMessages.emplace_back(welcomeMessage);

        players.push_back(msg.connection);
    }


    return returnMessages;
}

RoomDeque GameRoom::checkQuitters(const jsonMsg& msg) {
    RoomDeque goodbyes;
    if (msg.messageContent != command::QUIT) {
        return goodbyes;
    }

    auto removal = std::remove_if(players.begin(), players.end(),
                                  [&msg](networking::Connection player) { return player == msg.connection; });

    if (removal == players.end()) {
        return goodbyes;
    }

    players.erase(removal, players.end());
    jsonMsg goodbyeMsg = {.messageContent = "Disconnected from GameRoom", .gameRoom = this->roomCode, .connection = msg.connection};
    goodbyes.insert(goodbyeMsg);

    if (players.empty() || msg.connection == this->owner) {
        goodbyes.status = FINISHED;
    }

    return goodbyes;
}

