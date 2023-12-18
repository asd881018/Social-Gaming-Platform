
#pragma once

#include <string>
#include <vector>

class Player {
public:
    Player(const  std::string& name);
    void setChoice(const std::string& choice);
    
    std::string getName() const;
    std::string getChoice() const; 

    // Add other player-related methods and data members...
private:
    std::string playerName;
    std::string playerChoice;
    int wins;
};