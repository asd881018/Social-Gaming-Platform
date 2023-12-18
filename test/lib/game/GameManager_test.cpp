#include <gtest/gtest.h>
#include "GameScanner.h"

class GameManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

TEST_F(GameManagerTest, ScanGames) {
    GameScanner gameManager;
    gameManager.scanGames();

    std::vector<std::string> expectedGameNames = {"hello-tree-sitter", "rock-paper-scissors"};

    for (const auto& expectedName : expectedGameNames) {
        // Check if expected games exists in the GameScanner's list
        bool found = false;
        for (const auto& game : gameManager.viewGames()) {
            if (game.getName() == expectedName) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Expected game with name " << expectedName << " not found.";
    }
}