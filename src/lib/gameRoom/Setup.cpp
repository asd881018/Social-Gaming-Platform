#include "Setup.h"
#include "GameScanner.h"
#include "StringResources.h"

#include <sstream>
#include <iostream>

roomState::Setup::Setup(const std::vector<GameInfo>& gameInfos, DataBucket* dataBucket)
    : catalogue(gameInfos), dataBucket(dataBucket), sentGameOptions(false) {
    if (dataBucket) {
        // Example logic: Load game configurations into DataBucket
        // for (const auto& gameInfo : gameInfos) {
        //     dataBucket->readGameFileIntoDataBucket(gameInfo.getFilePath());
        // }
    }
}

// Constructor when DataBucket is not provided
roomState::Setup::Setup(const std::vector<GameInfo>& gameInfos) : catalogue(gameInfos), dataBucket(nullptr) {
    // No DataBucket provided, handle accordingly
}

RoomDeque roomState::Setup::poll(const jsonMsg& roomMessages, GameRoom& room) {
    RoomDeque returnMessages;

    if (roomMessages.messageContent == command::START) {
        room.changeState(std::make_unique<RunningGame>(GameParams{.game = catalogue.at(0), .gameInfos = catalogue, .players = room.viewPlayers()}));
        std::for_each(room.viewPlayers().begin(), room.viewPlayers().end(), [&returnMessages, &room] (auto c) {
            jsonMsg insertMessage{.messageContent = "Starting new game!",  .gameRoom = room.getRoomCode(), .connection = c};
            returnMessages.insert(insertMessage);
        });
    }

    // Assuming roomMessage is structured in a way that you can extract relevant information from it
    // Example: int gameChoice = roomMessage.getGameChoice();
    // Implement your logic here to extract the game choice or other necessary information from roomMessage

    int gameChoice = -1; // Extract the game choice from roomMessage
    if (gameChoice >= 0 && gameChoice < static_cast<int>(catalogue.size())) {
        const GameInfo& selectedGame = catalogue.at(gameChoice);

        if (dataBucket) {
            auto config = dataBucket->getWholeJSON(ImmutableKey(selectedGame.getName()));
            if (config) {
                generateConfigurationPrompts(selectedGame.getName(), dataBucket, room.viewOwner());
                // Additional logic for handling configuration responses...
            }

            room.changeState(std::make_unique<RunningGame>(GameParams{.game = catalogue.at(0), .gameInfos = catalogue, .players = room.viewPlayers()}));
            jsonMsg startGameMessage; // Populate with start game details
            returnMessages.insert(startGameMessage);
        }
    }

    return returnMessages;
}

jsonMsg roomState::Setup::generateGameSelectionMessage(const networking::Connection& connection) {
    std::stringstream builder;
    builder << "Select the game you want to play\n";

    unsigned int index = 0;
    std::for_each(catalogue.begin(), catalogue.end(), [&builder, &index](const auto& game) {
        builder << index++ << ". " << game.getName() << "\n";
    });

    // Create a jsonMsg object and set its messageContent field
    jsonMsg jsonMessage;
    jsonMessage.messageContent = builder.str();
    // Set other necessary fields of jsonMsg as required, for example:
    // jsonMessage.connection = connection;
    // Add other field initializations as needed

    return {};
}



void roomState::Setup::generateConfigurationPrompts(const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection) {
    if (!dataBucket) {
        std::cerr << "DataBucket is null" << std::endl;
        return;
    }

    auto gameConfigOpt = dataBucket->getWholeJSON(ImmutableKey(gameKey));
    if (!gameConfigOpt.has_value()) {
        std::cerr << "Error: Game configuration not found for key: " << gameKey << std::endl;
        return;
    }

    nlohmann::json& gameConfig = gameConfigOpt.value();

    if (gameConfig.contains("configuration") && gameConfig["configuration"].is_object()) {
        auto& configuration = gameConfig["configuration"];

        // Handling 'player range' configuration
        if (configuration.contains("player range")) {
            handlePlayerRangeConfig(configuration, gameKey, dataBucket, connection);
        }

        // Handling 'audience' configuration
        if (gameConfig.contains("configuration") && gameConfig["configuration"].is_object() &&
            gameConfig["configuration"].contains("audience")) {

            handleAudienceConfig(gameConfig["configuration"], gameKey, dataBucket, connection);
        }

        // Handling 'rounds' configuration from 'setup'
        if (gameConfig.contains("configuration") && gameConfig["configuration"].is_object() &&
            gameConfig["configuration"].contains("setup") &&
            gameConfig["configuration"]["setup"].is_object() &&
            gameConfig["configuration"]["setup"].contains("rounds")) {

            auto gameMessage = networking::Message{connection, ""};
        }
    }
}


// Function to handle player range configuration
void roomState::Setup::handlePlayerRangeConfig(nlohmann::json& configuration, const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection) {
    if (!dataBucket) {
        std::cerr << "DataBucket is null" << std::endl;
        return;
    }


    int minPlayers = configuration["player range"][0];
    int maxPlayers = configuration["player range"][1];
    std::string messageText = "How many players do you want to have in the room? (" +
                              std::to_string(minPlayers) + " - " + std::to_string(maxPlayers) + ")";
    networking::Message playerRangeMessage{connection, messageText};
    // Send the message
    // networkManager.sendMessage(playerRangeMessage);

    std::string playerResponse = waitForResponse();  // Pseudocode for waiting for a response
    int playerCount = std::stoi(playerResponse);    // Convert response to integer
    if (playerCount >= minPlayers && playerCount <= maxPlayers) {
        configuration["player range"] = playerCount;  // Modifying directly
        dataBucket->updateJSON(ImmutableKey(gameKey), configuration);
    }

}

void roomState::Setup::handleAudienceConfig(nlohmann::json& configuration, const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection) {
    if (!dataBucket) {
        std::cerr << "DataBucket is null" << std::endl;
        return;
    }
    std::string messageText = "Is audience participation allowed? (yes/no)";
    networking::Message audienceMessage{connection, messageText};
    // Send the message
    // networkManager.sendMessage(audienceMessage);

    std::string audienceResponse = waitForResponse();
    bool audienceAllowed = (audienceResponse == "yes");
    configuration["audience"] = audienceAllowed;
    dataBucket->updateJSON(ImmutableKey(gameKey), configuration);
}

void roomState::Setup::handleRoundsConfig(nlohmann::json& configuration, const std::string& gameKey, DataBucket* dataBucket, const networking::Connection& connection) {
    if (!dataBucket) {
        std::cerr << "DataBucket is null" << std::endl;
        return;
    }
    int minRounds = configuration["setup"]["rounds"]["range"][0];
    int maxRounds = configuration["setup"]["rounds"]["range"][1];
    std::string messageText = "How many rounds to play? (" +
                              std::to_string(minRounds) + " - " + std::to_string(maxRounds) + ")";
    networking::Message roundsMessage{connection, messageText};
    // Send the message
    // networkManager.sendMessage(roundsMessage);

    std::string roundsResponse = waitForResponse();
    int roundsCount = std::stoi(roundsResponse);     // Convert response to integer
    if (roundsCount >= minRounds && roundsCount <= maxRounds) {
        configuration["setup"]["rounds"]["range"] = roundsCount;
        dataBucket->updateJSON(ImmutableKey(gameKey), configuration);
    }
}



std::string roomState::Setup::waitForResponse() {
    // Implement the logic to wait for a response from the player.
    // This will depend on how networking and message handling is set up.
    // Return the player's response as a string.

    return "haha";
}

