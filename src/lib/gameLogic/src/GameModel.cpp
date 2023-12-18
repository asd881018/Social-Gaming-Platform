#include "GameModel.h"

GameModel::GameModel(const Configuration& config) : gameConfig(config) {
    // Additional setup code if needed
}

void GameModel::initializeGame() {
    // Extract game configuration
    std::string name = gameConfig.name;
    ts::Extent<int32_t> playerRange = gameConfig.player_range;
    bool audience = gameConfig.audience;
    // Extract additional setup parameters if needed

    // Initialize players
    for (int i = 1; i <= playerRange.end; ++i) {
        players.push_back("Player " + std::to_string(i));
        playerChoices["Player " + std::to_string(i)] = "";
        playerWins["Player " + std::to_string(i)] = 0;
    }

    // Extract answers or other game elements
    // Modify the following code to extract and initialize your game-specific data
    // Example code:
    // answers = gameConfig.setup.rounds_prompt_tmp;  TODO
    // Stub For Now:

    // Create choices and fill with random "a", "b", or "c"
    std::vector<std::string> possibleAnswers = {"a", "b", "c"};
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, 2);

    for (int i = 0; i < players.size(); ++i) {
        int randomIndex = distribution(generator);
        answers.push_back(possibleAnswers[randomIndex]);
    }
    

    // Additional initialization code as needed
}

void GameModel::playRound(int round) {
    // Simulate a round of the game
    std::vector<std::string> choices = answers;

    for (const std::string& player : players) {
        // Simulate player's choice (you can replace this with actual player input)
        int choiceIndex = rand() % choices.size();
        std::string playerChoice = choices[choiceIndex];
        playerChoices[player] = playerChoice;
    }

    // Determine the winners of the round and update game state
    // Modify this section to implement your game's logic

    // Print round results
    std::cout << "Round " << round << " results:" << std::endl;
    for (const std::string& player : players) {
        std::cout << player << " chose " << playerChoices[player] << std::endl;
    }
    std::cout << "Winners: ";
    for (const std::string& winner : winners) {
        std::cout << winner << " ";
    }
    std::cout << std::endl;
}

const Configuration& GameModel::getGameConfig() const {
    return gameConfig;
}
