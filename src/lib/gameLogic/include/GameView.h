
#pragma once

#include "GameModel.h"
#include "Player.h" 
#include <vector>
#include <string>
#include <iostream>

class GameView {
public:
    void displayGameInfo(const GameModel& gameModel);
    void displayRoundResults(int round, const std::vector<Player>& players, const std::vector<std::string>& winners);
    void getUserInput(GameModel& gameModel);

    // Add additional methods for displaying game information as needed
private:
    GameModel model; // Use the GameModel class as a member
};