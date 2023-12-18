#pragma once
#include <map>
#include <string>
#include <variant>
#include <optional>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

// #include "configuration.h"
// Immutable Key class
class ImmutableKey {
public:
    struct Hash {
        std::size_t operator()(const ImmutableKey& key) const {
            return std::hash<std::string>()(key.get());
        }
    };
private:
    const std::string value;
public:
    explicit ImmutableKey(const std::string& val) : value(val) {}
    ImmutableKey& operator=(const ImmutableKey&) = delete;
    const std::string& get() const { return value; }
    bool operator<(const ImmutableKey& other) const { return value < other.value; }
    bool operator==(const ImmutableKey& other) const { return value == other.value; }
    friend std::ostream& operator<<(std::ostream& os, const ImmutableKey& key);
};

struct PlayerBucket {
    int wins = 0;
    std::string connectionId;
};

struct AudienceBucket {
    std::string connectionId;
    
};

class DataBucket {
private:
    using DataVariant = std::variant<bool, int, std::string, nlohmann::json>;
    std::unordered_map<ImmutableKey, DataVariant, ImmutableKey::Hash> data;
    std::unordered_map<std::string, PlayerBucket> playerBuckets;
    std::unordered_map<std::string, AudienceBucket> audienceBuckets; 

public:
    DataBucket();

    // Insert/Update functions
    template <typename T>
    void insert(const ImmutableKey& key, T value);

    void insertEnum(const ImmutableKey& key, const std::map<std::string, std::string>& enumChoices);
    void insertJSON(const ImmutableKey& key, const json& value);

    // Retrieve functions
    std::optional<bool> getBoolean(const ImmutableKey& key) const;
    std::optional<int> getInteger(const ImmutableKey& key) const;
    std::optional<std::string> getString(const ImmutableKey& key) const;
    std::optional<nlohmann::json> getEnum(const ImmutableKey& key) const;
    std::optional<json> getJSON(const ImmutableKey& immutableKey, const std::string& path, const std::string& searchKey) const;
    //helper function for getting the Whole JSON value
    std::optional<json> getWholeJSON(const ImmutableKey& key) const;
    //update functions
    template <typename T>
    void update(const ImmutableKey& key, T value);

    void updateEnum(const ImmutableKey& key, const std::map<std::string, std::string>& enumChoices);
    void updateJSON(const ImmutableKey& key, const json& newValue);
    // Utility
    bool contains(const ImmutableKey& key) const;

    bool readGameFileIntoDataBucket(const std::filesystem::path& gameFilePath, DataBucket& dataBucket);

    // Methods for handling configurations, constants, and variables
    void setConfiguration(const nlohmann::json& config);
    void setConstants(const nlohmann::json& constants);
    void setVariables(const nlohmann::json& variables);

    // Methods for handling per-player and per-audience states
    void addPlayer(const std::string& playerId, const PlayerBucket& playerData);
    void updatePlayerState(const std::string& playerId, const PlayerBucket& newState);
    std::optional<PlayerBucket> getPlayerBucket(const std::string& playerId) const;

    void addAudienceMember(const std::string& audienceId, const AudienceBucket& audienceData);
    void updateAudienceState(const std::string& audienceId, const AudienceBucket& newState);
    std::optional<AudienceBucket> getAudienceBucket(const std::string& audienceId) const;

    // Methods to retrieve configurations, constants, and variables
    std::optional<nlohmann::json> getConfiguration() const;
    std::optional<nlohmann::json> getConstants() const;
    std::optional<nlohmann::json> getVariables() const;
};

inline std::ostream& operator<<(std::ostream& os, const ImmutableKey& key) {
    os << key.get();
    return os;
}

// Template specialization for inserting bool
template <>
void DataBucket::insert<bool>(const ImmutableKey& key, bool value);

// Template specialization for inserting int
template <>
void DataBucket::insert<int>(const ImmutableKey& key, int value);

// Template specialization for inserting std::string
template <>
void DataBucket::insert<std::string>(const ImmutableKey& key, std::string value);

// Template specialization for updating bool
template <>
void DataBucket::update<bool>(const ImmutableKey& key, bool value);

// Template specialization for updating int
template <>
void DataBucket::update<int>(const ImmutableKey& key, int value);

// Template specialization for updating std::string
template <>
void DataBucket::update<std::string>(const ImmutableKey& key, std::string value);
