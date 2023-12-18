#include "GameInfo.h"
#include "GameScanner.h"

#include <filesystem>
#include <iostream>

void GameScanner::scanGames() {

    std::filesystem::path currentDir = std::filesystem::current_path();
    std::filesystem::path folderPath = currentDir / "resource/games";

    if(std::filesystem::exists(folderPath)) {
        for (const auto& file : std::filesystem::directory_iterator(folderPath)) {
            if (file.path().extension() == ".game") {
                std::string name = file.path().stem().string();
                std::string filePath = file.path().string();

                GameInfo game(name, filePath);
                games.push_back(game);
            }
        }
    }else {
        std::cout << "Error: invalid file path" << std::endl;
    }


}

void GameScanner::printGamesInfo() const {
    std::cout << "List of available games:\n";
    for (const auto& game : games) {
        std::cout << "Name: " << game.getName() << ", Path: " << game.getFilePath() << "\n";
    }
}

const std::vector<GameInfo>& GameScanner::viewGames() const {
    return games;
}
