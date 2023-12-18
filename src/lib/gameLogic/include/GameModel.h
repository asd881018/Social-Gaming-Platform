// GameModel.h
#pragma once

#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <iostream>

#include "treeSitter.h"

class GameModel {
public:
    GameModel(const Configuration& config);  
    void initializeGame();  
    void playRound(int round);  
    const Configuration& getGameConfig() const;

private:
    Configuration gameConfig;  
    std::vector<std::string> players;
    std::map<std::string, std::string> playerChoices;
    std::map<std::string, int> playerWins;
    std::vector<std::string> answers;
    std::vector<std::string> winners;
};
