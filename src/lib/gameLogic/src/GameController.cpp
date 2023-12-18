#include "GameController.h"

GameController::GameController(const Configuration& config)
    : gameModel(config) {

    if (gameModel.getGameConfig().name.empty()) {
        // Handle the error (e.g., unable to read the game configuration)
        // For example: std::cerr << "Failed to read the game configuration file: " << gameConfigFile << std::endl;
    }
}

void GameController::runGame() {
    // Simulate multiple rounds of the game
    int numRounds = 5;  // You can change the number of rounds
    for (int round = 1; round <= numRounds; ++round) {
        gameModel.playRound(round);
    }
}

