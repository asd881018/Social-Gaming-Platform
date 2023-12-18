#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MessageProcessor.h"
#include "msgCreator.h"
#include "RoomCodeGenerator.h"
#include "GameRoom.h"
#include "StringResources.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include <string_view>

using namespace codeGenerator;
using namespace messageProcessor;
using ::testing::Return;

class MockState : public roomState::RoomState {
    RoomDeque poll(const jsonMsg& roomMessages, GameRoom& room) override {
        return {};
    }
};

class MessageProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        testCodeGenerator1 = RoomCodeGenerator(0, 2);
        testCodeGenerator2 = RoomCodeGenerator(0, 0);

        testRoomID = {20};
    }

    void TearDown() override {}
    
    RoomCodeGenerator testCodeGenerator1;
    RoomCodeGenerator testCodeGenerator2;
    std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> testGameRooms;
    std::deque<jsonMsg> testOutgoingMsgs;
    GameRoomID testRoomID;
    GameRoomID failRoomID;
};

// Tests that this function routes the passed in message to the right processors
TEST_F(MessageProcessorTest, TestProcessMessages) {
    jsonMsg incomingMsg1 = {.connection = {2}};
    jsonMsg incomingMsg2 = {.gameRoom = INVALID_ROOM_CODE, .connection = {3}};

    // tests that it gets routed to ProcessGameCreateMessage
    testOutgoingMsgs = processMessages(incomingMsg1, testGameRooms, testCodeGenerator2);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg1.connection);
    EXPECT_EQ(testOutgoingMsgs[0].gameRoom, INVALID_ROOM_CODE);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, OUT_OF_GAME_ROOMS_MSG);

    // tests that it gets routed to ProcessGameMessage
    testOutgoingMsgs = processMessages(incomingMsg2, testGameRooms, testCodeGenerator2);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg2.connection);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, INVALID_ROOM_CODE_MSG);
}

// Tests that We are able to process game create message i.e. create game rooms and create outgoing messages
// also tests behaviour when no more gameRooms can be made
TEST_F(MessageProcessorTest, TestProcessGameCreateMessage) {
    jsonMsg incomingMsg1 = {.connection = {2}};
    jsonMsg incomingMsg2 = {.connection = {5}};
    jsonMsg incomingMsg3 = {.connection = {10}};

    // tests that it can create a game rooms
    testOutgoingMsgs = processGameCreateMessage(incomingMsg1, testGameRooms, testCodeGenerator1);
    EXPECT_EQ(testGameRooms.size(), 1);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg1.connection);
    EXPECT_GT(testOutgoingMsgs[0].gameRoom, INVALID_ROOM_CODE);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, std::to_string(testOutgoingMsgs[0].gameRoom.id));
    EXPECT_NE(testGameRooms.find(testOutgoingMsgs[0].gameRoom), testGameRooms.end());

    testOutgoingMsgs = processGameCreateMessage(incomingMsg2, testGameRooms, testCodeGenerator1);
    EXPECT_EQ(testGameRooms.size(), 2);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg2.connection);
    EXPECT_GT(testOutgoingMsgs[0].gameRoom, INVALID_ROOM_CODE);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, std::to_string(testOutgoingMsgs[0].gameRoom.id));
    EXPECT_NE(testGameRooms.find(testOutgoingMsgs[0].gameRoom), testGameRooms.end());

    // tests behaviour when no more gameRooms can be created
    testOutgoingMsgs = processGameCreateMessage(incomingMsg3, testGameRooms, testCodeGenerator1);
    EXPECT_EQ(testGameRooms.size(), 2);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg3.connection);
    EXPECT_EQ(testOutgoingMsgs[0].gameRoom, INVALID_ROOM_CODE);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, OUT_OF_GAME_ROOMS_MSG);
    EXPECT_EQ(testGameRooms.find(testOutgoingMsgs[0].gameRoom), testGameRooms.end());
}

// holding off on implementing this until we understand how to do the closing of gameRooms better
TEST_F(MessageProcessorTest, TestProcessGameMessage) {
    jsonMsg incomingMsg1 = {.gameRoom = INVALID_ROOM_CODE, .connection = {2}};
    jsonMsg incomingMsg2 = {.gameRoom = {testRoomID}, .connection = {3}};
    jsonMsg incomingMsg3 = {.messageContent = command::QUIT, .gameRoom = {testRoomID}, .connection = {2}};

    testGameRooms.emplace(testRoomID, GameRoom(incomingMsg1.connection, testRoomID, std::make_unique<MockState>()));
    EXPECT_EQ(testGameRooms.size(), 1);

    // tests that it works when a message has a roomcode for a room that does not exists
    testOutgoingMsgs = processGameMessage(incomingMsg1, testGameRooms);
    EXPECT_EQ(testOutgoingMsgs.size(), 1);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg1.connection);
    EXPECT_EQ(testOutgoingMsgs[0].gameRoom, INVALID_ROOM_CODE);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, INVALID_ROOM_CODE_MSG);

    // tests that processing generally works as in you get a outgoing messages back
    testOutgoingMsgs = processGameMessage(incomingMsg2, testGameRooms);
    EXPECT_EQ(testOutgoingMsgs.size(), 1);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg2.connection);
    EXPECT_EQ(testOutgoingMsgs[0].gameRoom, testRoomID);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, "Welcome to the game!");

    // tests that the gameRoom processed the message properly and properly removes the gameRoom (closes it)
    EXPECT_EQ(testGameRooms.size(), 1);
    testOutgoingMsgs = processGameMessage(incomingMsg3, testGameRooms);
    EXPECT_EQ(testGameRooms.size(), 0);
    EXPECT_EQ(testOutgoingMsgs.size(), 1);
    EXPECT_EQ(testOutgoingMsgs[0].connection, incomingMsg3.connection);
    EXPECT_EQ(testOutgoingMsgs[0].gameRoom, testRoomID);
    EXPECT_EQ(testOutgoingMsgs[0].messageContent, "Disconnected from GameRoom");
}