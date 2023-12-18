#include <gtest/gtest.h>
#include "GameInfo.h"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
    void TearDown() override {
    }

    const std::string testName = "TestGame";
    const std::string testPath = "/path/to/TestGame";

};

TEST_F(GameTest, ConstructorTest) {

    GameInfo game(testName, testPath);

    EXPECT_EQ(game.getName(), testName);
    EXPECT_EQ(game.getFilePath(), testPath);
}

TEST_F(GameTest, GetName) {

    GameInfo game(testName, testPath);

    EXPECT_EQ(game.getName(), testName);
}

TEST_F(GameTest, GetFilePath) {

    GameInfo game(testName, testPath);

    EXPECT_EQ(game.getFilePath(), testPath);
}