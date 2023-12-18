#pragma once

#include "GameInfo.h"

#include <vector>

class GameScanner {
public:
    void scanGames();

    void printGamesInfo() const;

    const std::vector<GameInfo>& viewGames() const;

private:
    std::vector<GameInfo> games;
};
