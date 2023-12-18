#include "GameInfo.h"

GameInfo::GameInfo(const std::string& name, const std::string& filePath) : name(name), filePath(filePath) {}

const std::string GameInfo::getName() const {
    return name;
}

std::string GameInfo::getFilePath() const {
    return filePath;
}