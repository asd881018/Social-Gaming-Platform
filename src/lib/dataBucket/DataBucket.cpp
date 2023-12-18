#include "DataBucket.h"
#include <typeinfo>
#include <iostream>
#include "GameScanner.h"

DataBucket::DataBucket() {}

template <>
void DataBucket::insert<const char*>(const ImmutableKey& key, const char* value) {
    data[key] = std::string(value);
}

template <>
void DataBucket::insert<bool>(const ImmutableKey& key, bool value) {
    data[key] = value;
}

template <>
void DataBucket::insert<int>(const ImmutableKey& key, int value) {
    data[key] = value;
}


void DataBucket::insertEnum(const ImmutableKey& key, const std::map<std::string, std::string>& enumChoices) {
    nlohmann::json jsonEnumChoices(enumChoices);
    data[key] = jsonEnumChoices;
}

void DataBucket::insertJSON(const ImmutableKey& key, const json& value) {
    data[key] = value;
}

std::optional<bool> DataBucket::getBoolean(const ImmutableKey& key) const {
    if (data.count(key) && std::holds_alternative<bool>(data.at(key))) {
        return std::get<bool>(data.at(key));
    }
    return {};
}

std::optional<int> DataBucket::getInteger(const ImmutableKey& key) const {
    if (data.count(key) && std::holds_alternative<int>(data.at(key))) {
        return std::get<int>(data.at(key));
    }
    return {};
}

std::optional<std::string> DataBucket::getString(const ImmutableKey& key) const {
    auto it = data.find(key);
    if (it != data.end()) {
        const auto& var = it->second;
        if (std::holds_alternative<std::string>(var)) {
            return std::get<std::string>(var);
        } else {
            // Manual type checking for debugging purposes
            if (std::holds_alternative<int>(var)) {
                std::cerr << "Data at key is not a string, but an int. Key: " << key.get() << std::endl;
            } else if (std::holds_alternative<bool>(var)) {
                std::cerr << "Data at key is not a string, but a bool. Key: " << key.get() << std::endl;
            } else if (std::holds_alternative<nlohmann::json>(var)) {
                std::cerr << "Data at key is not a string, but a json. Key: " << key.get() << std::endl;
            } else {
                // Add more checks if there are more types
                std::cerr << "Data at key is an unknown type. Key: " << key.get() << std::endl;
            }
            return std::nullopt;
        }
    } else {
        std::cerr << "No data at key: " << key.get() << std::endl;
    }
    return std::nullopt;
}




std::optional<nlohmann::json> DataBucket::getEnum(const ImmutableKey& key) const {
    if (data.count(key) && std::holds_alternative<nlohmann::json>(data.at(key))) {
        // Explicitly construct an std::optional<nlohmann::json> to resolve ambiguity
        return std::optional<nlohmann::json>{std::get<nlohmann::json>(data.at(key))};
    }
    return std::nullopt;
}

std::optional<json> DataBucket::getJSON(const ImmutableKey& immutableKey, const std::string& path, const std::string& searchKey) const {
    auto it = data.find(immutableKey);
    if (it == data.end()) {
        std::cerr << "Error: Key '" << immutableKey << "' not found in the data bucket." << std::endl;
        return std::nullopt; // Key not found in the data bucket
    }

    // Directly return the whole JSON object if both path and searchKey are empty
    if (path.empty() && searchKey.empty()) {
        if (std::holds_alternative<json>(it->second)) {
            return std::optional<json>(std::get<json>(it->second));
        } else {
            std::cerr << "Error: Data associated with key '" << immutableKey << "' is not a JSON object." << std::endl;
            return std::nullopt; // The associated data is not a JSON object
        }
    }

    // If we have a path, let's traverse it
    const json* current = &std::get<json>(it->second);
    if (!path.empty()) {
        std::istringstream pathStream(path);
        std::string segment;
        while (std::getline(pathStream, segment, '.')) {
            if (current->is_object() && current->contains(segment)) {
                current = &(*current)[segment];
            } else {
                std::cerr << "Error: Segment '" << segment << "' not found or not an object within the JSON path '" << path << "'." << std::endl;
                return std::nullopt; // Segment not found or not an object within the JSON
            }
        }
    }

    // At the end of the path, search for the key if it's provided
    if (!searchKey.empty()) {
        if (current->is_object() && current->contains(searchKey)) {
            return std::optional<json>{(*current)[searchKey]};
        } else if (current->is_array()) {
            for (const auto& item : *current) {
                if (item.is_object() && item.contains(searchKey)) {
                    return std::optional<json>{item[searchKey]};
                }
            }
        }
        std::cerr << "Error: Search key '" << searchKey << "' not found within the JSON object or array." << std::endl;
        return std::nullopt; // Search key not found within the JSON object or array
    }

    // If searchKey is empty, return the JSON found at the end of the path
    return std::optional<json>{*current};
}


std::optional<json> DataBucket::getWholeJSON(const ImmutableKey& key) const {
    return getJSON(key, "", "");
}

template <>
void DataBucket::update<const char*>(const ImmutableKey& key, const char* value) {
    if (data.count(key) && std::holds_alternative<std::string>(data[key])) {
        data[key] = std::string(value);
    }
}

template <>
void DataBucket::update<bool>(const ImmutableKey& key, bool value) {
    if (data.count(key) && std::holds_alternative<bool>(data[key])) {
        data[key] = value;
    }
}

template <>
void DataBucket::update<int>(const ImmutableKey& key, int value) {
    if (data.count(key) && std::holds_alternative<int>(data[key])) {
        data[key] = value;
    }
}
void DataBucket::updateEnum(const ImmutableKey& key, const std::map<std::string, std::string>& enumChoices) {
    nlohmann::json jsonEnumChoices(enumChoices);
    if (data.count(key) && std::holds_alternative<json>(data[key])) {
        data[key] = jsonEnumChoices;
    }
}

void DataBucket::updateJSON(const ImmutableKey& key, const json& newValue) {
    auto it = data.find(key);
    if (it == data.end()) {
        std::cerr << "Error: Key does not exist in DataBucket" << std::endl;
        return; // Exit the function if the key doesn't exist
    }

    if (std::holds_alternative<json>(it->second)) {
        it->second = newValue;
    } else {
        std::cerr << "Error: Existing data at key is not a JSON object" << std::endl;
    }
}
bool DataBucket::contains(const ImmutableKey& key) const {
    return data.count(key);
}

bool readGameFileIntoDataBucket(const GameInfo& gameInfo, DataBucket& dataBucket) {
    std::filesystem::path gameFilePath = gameInfo.getFilePath();

    if (!std::filesystem::exists(gameFilePath) || gameFilePath.extension() != ".game") {
        std::cerr << "Error: Invalid game file path" << std::endl;
        return false;
    }

    std::ifstream gameFile(gameFilePath);
    if (!gameFile.is_open()) {
        std::cerr << "Error: Unable to open game file" << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << gameFile.rdbuf();
    std::string gameFileContents = buffer.str();

    try {
        // Mock parsedContents based on .game file format
        nlohmann::json parsedContents = {
            {"configuration", {
                {"name", "Rock, Paper, Scissors"},
                {"player range", {2, 4}},
                {"audience", false},
                {"setup", {
                    {"rounds", {
                        {"kind", "integer"},
                        {"prompt", "The number of rounds to play"},
                        {"range", {1, 20}}
                    }}
                }}
            }},
            {"constants", {
                {"weapons", {
                    {{"name", "Rock"}, {"beats", "Scissors"}},
                    {{"name", "Paper"}, {"beats", "Rock"}},
                    {{"name", "Scissors"}, {"beats", "Paper"}}
                }}
            }},
            {"variables", {
                {"winners", nlohmann::json::array()}
            }},
            {"per-player", {
                {"wins", 0}
            }},
        };

        ImmutableKey key(gameFilePath.stem().string()); // Key based on the game name
        dataBucket.insertJSON(key, parsedContents);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing .game file: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void DataBucket::setConfiguration(const nlohmann::json& config) {
    data[ImmutableKey("configuration")] = config;
}

void DataBucket::setConstants(const nlohmann::json& constants) {
    data[ImmutableKey("constants")] = constants;
}

void DataBucket::setVariables(const nlohmann::json& variables) {
    data[ImmutableKey("variables")] = variables;
}

void DataBucket::addPlayer(const std::string& playerId, const PlayerBucket& playerData) {
    playerBuckets[playerId] = playerData;
}

void DataBucket::updatePlayerState(const std::string& playerId, const PlayerBucket& newState) {
    auto it = playerBuckets.find(playerId);
    if (it != playerBuckets.end()) {
        it->second = newState;
    } else {
        std::cerr << "Error: Player ID not found: " << playerId << std::endl;
    }
}

std::optional<PlayerBucket> DataBucket::getPlayerBucket(const std::string& playerId) const {
    auto it = playerBuckets.find(playerId);
    if (it != playerBuckets.end()) {
        return it->second;
    }
    return std::nullopt;
}

void DataBucket::addAudienceMember(const std::string& audienceId, const AudienceBucket& audienceData) {
    audienceBuckets[audienceId] = audienceData;
}

void DataBucket::updateAudienceState(const std::string& audienceId, const AudienceBucket& newState) {
    auto it = audienceBuckets.find(audienceId);
    if (it != audienceBuckets.end()) {
        it->second = newState;
    } else {
        std::cerr << "Error: Audience ID not found: " << audienceId << std::endl;
    }
}

std::optional<AudienceBucket> DataBucket::getAudienceBucket(const std::string& audienceId) const {
    auto it = audienceBuckets.find(audienceId);
    if (it != audienceBuckets.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<nlohmann::json> DataBucket::getConfiguration() const {
    auto it = data.find(ImmutableKey("configuration"));
    if (it != data.end() && std::holds_alternative<nlohmann::json>(it->second)) {
        return std::get<nlohmann::json>(it->second);
    }
    return std::nullopt;
}

std::optional<nlohmann::json> DataBucket::getConstants() const {
    auto it = data.find(ImmutableKey("constants"));
    if (it != data.end() && std::holds_alternative<nlohmann::json>(it->second)) {
        return std::get<nlohmann::json>(it->second);
    }
    return std::nullopt;
}

std::optional<nlohmann::json> DataBucket::getVariables() const {
    auto it = data.find(ImmutableKey("variables"));
    if (it != data.end() && std::holds_alternative<nlohmann::json>(it->second)) {
        return std::get<nlohmann::json>(it->second);
    }
    return std::nullopt;
}

