#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GameRoom.h"
#include "GameRunner.h"
#include "msgCreator.h"
#include "StringResources.h"

class GameRunnerTest : public ::testing::Test {
protected:
    GameInfo info = GameInfo{"test", "test"};
    std::vector<GameInfo> test = {info};
    networking::Connection ownerConnection = {1234};
    GameRoom gameRoom = GameRoom(ownerConnection, {1111}, std::make_unique<roomState::RunningGame>(GameParams{.game = info, .gameInfos = test, .players = {ownerConnection}}));
};

TEST_F(GameRunnerTest, BasicTest) {
    jsonMsg startMessage = {.messageContent = command::START, .connection = ownerConnection};
    auto runnerState = gameRoom.poll(startMessage).messages;
    ASSERT_EQ(16, runnerState.size()) ;
}

TEST_F(GameRunnerTest, StartGameTest) {
    GameRoom startGameRoom = GameRoom(ownerConnection, {1111}, std::make_unique<roomState::Setup>(test));

    jsonMsg startMessage = {.messageContent = command::START, .connection = ownerConnection};
    auto result = startGameRoom.poll(startMessage).messages;

    ASSERT_EQ(1, result.size());
    auto message = result.at(0);
    EXPECT_EQ(ownerConnection, message.connection);
    EXPECT_EQ("Starting new game!", message.messageContent);

    auto runnerState = startGameRoom.poll(startMessage).messages;
    ASSERT_EQ(16, runnerState.size()) ;
}