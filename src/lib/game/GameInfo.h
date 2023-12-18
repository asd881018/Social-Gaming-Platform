#pragma once

#include <string>


class GameInfo {
public:
    GameInfo(const std::string& name, const std::string& path);
    GameInfo() = default; // todo

    const std::string getName() const;
    std::string getFilePath() const;

private:
    std::string name;
    std::string filePath;
};
