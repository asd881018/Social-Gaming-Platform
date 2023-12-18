#pragma once

#include "GameModel.h"

class GameController {
public:
    GameController(const Configuration& config);  // Constructor
    void runGame();

private:
    GameModel gameModel;
};