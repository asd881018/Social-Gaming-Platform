#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GameRoom.h"
#include "msgCreator.h"
#include "StringResources.h"

class MockState : public roomState::RoomState {
    RoomDeque poll(const jsonMsg& roomMessages, GameRoom& room) override {
        return {};
    }

};

class GameRoomTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    networking::Connection ownerConnection = networking::Connection{1234};
    networking::Connection secondPlayerConnection = networking::Connection{1114};
    GameRoomID gameRoomId = {1337};
    GameRoom gameRoom = GameRoom(ownerConnection, gameRoomId, std::make_unique<MockState>());
};

TEST_F(GameRoomTest, OneConnectionOnCreationTest) {
    EXPECT_EQ(ownerConnection, gameRoom.viewOwner());
    EXPECT_EQ(1, gameRoom.viewPlayers().size());
    EXPECT_EQ(ownerConnection, gameRoom.viewPlayers().at(0));
}

TEST_F(GameRoomTest, UserJoinTest) {
    jsonMsg joinMsg{};
    joinMsg.connection = secondPlayerConnection;

    RoomDeque result = gameRoom.poll(joinMsg);
    auto messages = result.messages;
    ASSERT_EQ(RoomExecution::OK, result.status);
    ASSERT_EQ(1, messages.size());
    auto singleMessage = messages.at(0);

    EXPECT_EQ(secondPlayerConnection, singleMessage.connection);
    EXPECT_EQ(gameRoomId, singleMessage.gameRoom);
    EXPECT_EQ("Welcome to the game!", singleMessage.messageContent);
    EXPECT_EQ(2, gameRoom.viewPlayers().size());


    jsonMsg genericMsg{.messageContent = "Generic message", .gameRoom = gameRoomId, .connection = secondPlayerConnection};
    messages = gameRoom.poll(joinMsg).messages;
    ASSERT_EQ(0, messages.size());
}

TEST_F(GameRoomTest, OwnerQuitTest) {
    gameRoom.poll({.connection = secondPlayerConnection});

    jsonMsg quitMsg = {.messageContent = command::QUIT , .gameRoom = gameRoomId, .connection = ownerConnection};
    RoomDeque result = gameRoom.poll(quitMsg);

    EXPECT_EQ(RoomExecution::FINISHED, result.status);
    ASSERT_EQ(1, result.messages.size());
    auto singleMessage = result.messages.at(0);

    EXPECT_EQ(ownerConnection, singleMessage.connection);
    EXPECT_EQ(gameRoomId, singleMessage.gameRoom);
    EXPECT_EQ(1, gameRoom.viewPlayers().size());
}

TEST_F(GameRoomTest, UserQuitTest) {
    gameRoom.poll({.connection = secondPlayerConnection});
    jsonMsg quitMsg = {.messageContent = command::QUIT , .gameRoom = gameRoomId, .connection = secondPlayerConnection};
    RoomDeque result = gameRoom.poll(quitMsg);

    EXPECT_EQ(RoomExecution::OK, result.status);
    ASSERT_EQ(1, result.messages.size());
    auto singleMessage = result.messages.at(0);

    EXPECT_EQ(secondPlayerConnection, singleMessage.connection);
    EXPECT_EQ(gameRoomId, singleMessage.gameRoom);
    EXPECT_EQ(1, gameRoom.viewPlayers().size());
}