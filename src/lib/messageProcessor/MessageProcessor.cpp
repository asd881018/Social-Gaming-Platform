#include "MessageProcessor.h"

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

using namespace roomState;
using networking::Message;
using codeGenerator::RoomCodeGenerator;

namespace messageProcessor{

// handles the processing of any messages
std::deque<jsonMsg> processMessages(const jsonMsg &message, std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> &gameRooms, RoomCodeGenerator &roomCodeGenerator) {
    std::deque<jsonMsg> outgoingMessages;

    // process each jsonMsg - we have decided the gameServer directly handles only 2 types of messages
    // 2 types of processes - 1 for creating game rooms & 1 for game rooms handle the processing
    if (message.gameRoom == NO_ROOM_ID) {
        // if message is not related to an existing gameRoom it is a gameCreate message
        outgoingMessages = processGameCreateMessage(message, gameRooms, roomCodeGenerator);
    } else {
        // if it does have a room code then it is considered a message that is handled by a gameRoom
        outgoingMessages = processGameMessage(message, gameRooms);
    }

    return outgoingMessages;
}

// handles the processing of creating a game room this just adds a gameRoom to the gameRooms vector with the message sender as the GameOwner
std::deque<jsonMsg> processGameCreateMessage(const jsonMsg &message, std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> &gameRooms, RoomCodeGenerator &roomCodeGenerator) {
    std::deque<jsonMsg> outgoingMessages;
    GameRoomID newRoomCode = roomCodeGenerator.generateCode();

    // checks if there are any more codes for creating a game room
    if (newRoomCode == INVALID_ROOM_CODE) {
        outgoingMessages.push_back({.messageContent = OUT_OF_GAME_ROOMS_MSG, .gameRoom = INVALID_ROOM_CODE, .connection = message.connection});
        return outgoingMessages;
    } 

    // creates a new game room in the set up state
    GameScanner gameManager;
    auto setUp = std::make_unique<Setup>(gameManager.viewGames());
    
    gameRooms.emplace(newRoomCode, GameRoom({message.connection}, newRoomCode, std::move(setUp)));

    // creates a outgoing message to the client letting them know game was created succesfully and what the roomcode is    
    jsonMsg outgoingMessage = {.messageContent = std::to_string(newRoomCode.id), .gameRoom = newRoomCode, .connection = message.connection};

    outgoingMessages.emplace_back(outgoingMessage);

    return outgoingMessages;
}

// handles the processing of any messages related to a gameRoom this includes things like joining a gameRoom, quiting a game room, and 
// getting user input for a game room. 
std::deque<jsonMsg> processGameMessage(const jsonMsg &message, std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> &gameRooms) {

    // TODO the gameRoom will also have a way of indicating that it has closed as needs to be removed from the gameRoom
    // figure out how to do some of the housekeeping for the gameRooms closing
    std::deque<jsonMsg> outgoingMessages;
    // checks if the room code actually matches to a real gameRoom
    if (gameRooms.find(message.gameRoom) == gameRooms.end()) {
        outgoingMessages.push_back({.messageContent = INVALID_ROOM_CODE_MSG, .gameRoom = INVALID_ROOM_CODE, .connection=message.connection});
        return outgoingMessages;
    }

    RoomDeque roomDeque = gameRooms.find(message.gameRoom)->second.poll(message);
    outgoingMessages = roomDeque.messages;
    if (roomDeque.status == RoomExecution::FINISHED) {
        gameRooms.erase(message.gameRoom);
    }
    
    return outgoingMessages;
}
}
