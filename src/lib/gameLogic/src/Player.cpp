#include "Player.h"

Player::Player(const std::string& name) : playerName(name), wins(0) {
    // Initialize player-specific data here
}

void Player::setChoice(const std::string& choice) {
    playerChoice = choice;
}

std::string Player::getName() const {
    return playerName;
}

std::string Player::getChoice() const {
    return playerChoice; 
}

// Implement other player-related methods...