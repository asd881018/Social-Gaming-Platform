#include "GameView.h"

void GameView::displayGameInfo(const GameModel& gameModel) {
    // Implement how to display game information
}

void GameView::displayRoundResults(int round, const std::vector<Player>& players, const std::vector<std::string>& winners) {
    std::cout << "Round " << round << " results:" << std::endl;
    
    for (const Player& player : players) {
        std::cout << player.getName() << " chose " << player.getChoice() << std::endl;
    }
    
    std::cout << "Winners: ";
    for (const std::string& winner : winners) {
        std::cout << winner << " ";
    }
    std::cout << std::endl;
}

void GameView::getUserInput(GameModel& gameModel) {
    // Implement how to get user input
}

// Implement other view-related methods...