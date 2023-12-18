#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <unordered_set>

#include "GameRoom.h"
#include "Server.h"
#include "GameScanner.h"
#include "msgCreator.h"
#include "msgParser.h"
#include "Setup.h"
#include "GameInfo.h"
#include "GameRoom.h"
#include "RoomCodeGenerator.h"

namespace messageProcessor{
    inline const GameRoomID NO_ROOM_ID = {0};
    inline const std::string INVALID_ROOM_CODE_MSG = "Room with that RoomCode Does Not Exists.";
    inline const std::string OUT_OF_GAME_ROOMS_MSG = "No more gamerooms left on the server.";

    // handles the processing of any messages
    std::deque<jsonMsg> processMessages(const jsonMsg &message, std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> &gameRooms, codeGenerator::RoomCodeGenerator &roomCodeGenerator);

    // handles the processing of creating a game room this just adds a gameRoom to the gameRooms vector with the message sender as the GameOwner
    std::deque<jsonMsg> processGameCreateMessage(const jsonMsg &message, std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> &gameRooms, codeGenerator::RoomCodeGenerator &roomCodeGenerator);

    // handles the processing of any messages related to a gameRoom this includes things like joining a gameRoom, quiting a game room, and 
    // getting user input for a game room. 
    // TODO It also will handle the closing of a gameRoom i.e. removing it from the list of gameRooms
    std::deque<jsonMsg> processGameMessage(const jsonMsg &message, std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> &gameRooms);

}

#endif