#include "DataBucket.h"
#include "gtest/gtest.h"
#include <string>
#include <nlohmann/json.hpp>

class DataBucketTest : public ::testing::Test {
protected:
    DataBucket bucket;

    virtual void SetUp() {
        bucket = DataBucket();
    }

    virtual void TearDown() {
        // Clean up your DataBucket here if needed
    }
};

// Test insert and get for boolean values
TEST_F(DataBucketTest, HandleBooleanInsertAndGet) {
    ImmutableKey key("testBool"); // Updated
    bucket.insert(key, true);
    auto value = bucket.getBoolean(key);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), true);
}

// Test insert and get for integer values
TEST_F(DataBucketTest, HandleIntegerInsertAndGet) {
    ImmutableKey key("testInt"); // Updated
    bucket.insert(key, 42);
    auto value = bucket.getInteger(key);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 42);
}

// Test insert and get for string values
TEST_F(DataBucketTest, HandleStringInsertAndGet) {
    ImmutableKey key("testString"); // Updated
    bucket.insert(key, "Hello, World!");
    auto value = bucket.getString(key);

    ASSERT_TRUE(value.has_value());
    
    EXPECT_EQ(value.value(), "Hello, World!");
}

// Test insert and get for enum values
TEST_F(DataBucketTest, HandleEnumInsertAndGet) {
    ImmutableKey key("testEnum"); // Updated
    std::map<std::string, std::string> enumChoices = {{"key1", "value1"}, {"key2", "value2"}};
    bucket.insertEnum(key, enumChoices);
    auto value = bucket.getEnum(key);

    ASSERT_TRUE(value.has_value());
    nlohmann::json expected = enumChoices;
    EXPECT_EQ(value.value(), expected);
}

// Test update for boolean values
TEST_F(DataBucketTest, HandleBooleanUpdate) {
    ImmutableKey key("testBool"); // Updated
    bucket.insert(key, true);
    bucket.update(key, false);
    auto value = bucket.getBoolean(key);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), false);
}

// Test update for integer values
TEST_F(DataBucketTest, HandleIntegerUpdate) {
    ImmutableKey key("testInt"); // Updated
    bucket.insert(key, 42);
    bucket.update(key, 43);
    auto value = bucket.getInteger(key);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 43);
}

// Test update for string values
TEST_F(DataBucketTest, HandleStringUpdate) {
    ImmutableKey key("testString"); // Updated
    bucket.insert(key, "Hello");
    bucket.update(key, "Goodbye");
    auto value = bucket.getString(key);

    ASSERT_TRUE(value.has_value());
    
    EXPECT_EQ(value.value(), "Goodbye");
}

// Test contains method
TEST_F(DataBucketTest, HandleContains) {
    ImmutableKey key("testBool"); // Updated
    EXPECT_FALSE(bucket.contains(key));
    bucket.insert(key, true);
    EXPECT_TRUE(bucket.contains(key));
}

TEST_F(DataBucketTest, InsertJSON) {
    // Create the JSON to insert
    nlohmann::json configuration_json = {
        {"name", "Hello, tree-sitter"},
        {"playerRange", {2, 4}},
        {"audience", false},
        {"setup", {
            {"rounds", {
                {"kind", "integer"},
                {"prompt", "The number of rounds to play"},
                {"range", {1, 20}}
            }}
        }}
    };

    // Use a key for insertion
    ImmutableKey key("config_key");

    // Call the insertJSON method
    bucket.insertJSON(key, configuration_json);

    // No path and no search key, should return the entire JSON object
    auto retrieved_json = bucket.getJSON(key, "", "");
    ASSERT_TRUE(retrieved_json.has_value());
    EXPECT_EQ(retrieved_json.value(), configuration_json);
    //cout for debugging
    // std::cout << retrieved_json.value() << std::endl;
}


TEST_F(DataBucketTest, GetAllKeys) {
    // Insert the JSON object with the provided configuration
    nlohmann::json configuration_json = {
        {"name", "Hello, tree-sitter"},
        {"playerRange", {2, 4}},
        {"audience", false},
        {"setup", {
            {"rounds", {
                {"kind", "integer"},
                {"prompt", "The number of rounds to play"},
                {"range", {1, 20}}
            }}
        }}
    };

    // Use a key for insertion
    ImmutableKey key("config_key");
    bucket.insertJSON(key, configuration_json);

    // Retrieve the 'name' value
    auto name = bucket.getJSON(key, "", "name");
    ASSERT_TRUE(name.has_value());
    EXPECT_EQ(name.value(), "Hello, tree-sitter");

    // Retrieve the 'playerRange' value
    auto playerRange = bucket.getJSON(key, "", "playerRange");
    ASSERT_TRUE(playerRange.has_value());
    EXPECT_EQ(playerRange.value(), configuration_json["playerRange"]);

    // Retrieve the 'audience' value
    auto audience = bucket.getJSON(key, "", "audience");
    ASSERT_TRUE(audience.has_value());
    EXPECT_EQ(audience.value(), false);

    // Retrieve the 'setup' value
    auto setup = bucket.getJSON(key, "", "setup");
    ASSERT_TRUE(setup.has_value());
    EXPECT_EQ(setup.value(), configuration_json["setup"]);

    // Retrieve the 'rounds' value
    auto rounds = bucket.getJSON(key, "setup", "rounds");
    ASSERT_TRUE(rounds.has_value());
    EXPECT_EQ(rounds.value(), configuration_json["setup"]["rounds"]);

    // Retrieve the 'kind' value
    auto kind = bucket.getJSON(key, "setup.rounds", "kind");
    ASSERT_TRUE(kind.has_value());
    EXPECT_EQ(kind.value(), "integer");

    // Retrieve the 'prompt' value
    auto prompt = bucket.getJSON(key, "setup.rounds", "prompt");
    ASSERT_TRUE(prompt.has_value());
    EXPECT_EQ(prompt.value(), "The number of rounds to play");

    // Retrieve the 'range' value
    auto range = bucket.getJSON(key, "setup.rounds", "range");
    ASSERT_TRUE(range.has_value());
    EXPECT_EQ(range.value(), configuration_json["setup"]["rounds"]["range"]);

    // Output for debugging (if necessary)
    // std::cout << "Name: " << name.value() << std::endl;
    // std::cout << "Player Range: " << playerRange.value() << std::endl;
    // std::cout << "Audience: " << audience.value() << std::endl;
    // std::cout << "Setup: " << setup.value() << std::endl;
    // std::cout << "Rounds: " << rounds.value() << std::endl;
    // std::cout << "Kind: " << kind.value() << std::endl;
    // std::cout << "Prompt: " << prompt.value() << std::endl;
    // std::cout << "Range: " << range.value() << std::endl;
}

TEST(DataBucketUpdateJSON, UpdateExistingJSON) {
    DataBucket bucket;
    nlohmann::json initialJson = {{"key1", "value1"}};
    bucket.insertJSON(ImmutableKey("existing_json"), initialJson);
    
    nlohmann::json newValue = {
        {"name", "Hello, tree-sitter"},
        {"playerRange", {2, 4}},
        {"audience", false},
        {"setup", {
            {"rounds", {
                {"kind", "integer"},
                {"prompt", "The number of rounds to play"},
                {"range", {1, 20}}
            }}
        }}
    };
    bucket.updateJSON(ImmutableKey("existing_json"), newValue);

    auto updatedValue = bucket.getJSON(ImmutableKey("existing_json"), "", "");
    ASSERT_TRUE(updatedValue.has_value());
    EXPECT_EQ((*updatedValue)["name"], "Hello, tree-sitter");
}

// Test updating a non-existing key
TEST(DataBucketUpdateJSON, UpdateNonExistingKey) {
    DataBucket bucket;
    nlohmann::json newValue = {
        {"name", "Hello, tree-sitter"}
        // ... other fields if necessary
    };
    testing::internal::CaptureStderr();  // Start capturing stderr
    bucket.updateJSON(ImmutableKey("non_existing_key"), newValue);
    std::string output = testing::internal::GetCapturedStderr(); // Stop capturing stderr

    EXPECT_EQ(output, "Error: Key does not exist in DataBucket\n");
}

// Test updating a key that does not hold a JSON object
TEST(DataBucketUpdateJSON, UpdateKeyWithWrongType) {
    DataBucket bucket;
    bucket.insert<int>(ImmutableKey("existing_int"), 42);

    nlohmann::json newValue = {
        {"name", "Hello, tree-sitter"}
        // ... other fields if necessary
    };
    testing::internal::CaptureStderr();  // Start capturing stderr
    bucket.updateJSON(ImmutableKey("existing_int"), newValue);
    std::string output = testing::internal::GetCapturedStderr(); // Stop capturing stderr

    EXPECT_EQ(output, "Error: Existing data at key is not a JSON object\n");
}

TEST(DataBucketUpdateJSON, UpdateAudienceAndPrompt) {
    DataBucket bucket;
    nlohmann::json initialJson = {
        {"key1", "value1"},
        {"name", "Hello, tree-sitter"},
        {"playerRange", {2, 4}},
        {"audience", false},
        {"setup", {
            {"rounds", {
                {"kind", "integer"},
                {"prompt", "The number of rounds to play"},
                {"range", {1, 20}}
            }}
        }}
    };
    bucket.insertJSON(ImmutableKey("json_for_update"), initialJson);
    
    // Create a JSON object with the values to be updated
    nlohmann::json updatedFields = {
        {"audience", true},
        {"setup", {
            {"rounds", {
                {"prompt", "test changing the prompt"}
            }}
        }}
    };

    // Update the JSON in the bucket with the new values
    bucket.updateJSON(ImmutableKey("json_for_update"), updatedFields);

    // Retrieve the updated JSON
    auto updatedValue = bucket.getJSON(ImmutableKey("json_for_update"), "", "");
    
    // Check if the updated JSON is retrieved successfully
    ASSERT_TRUE(updatedValue.has_value());

    // Assert the new values for 'audience' and 'prompt'
    EXPECT_EQ((*updatedValue)["audience"], true);
    auto updatedPrompt = (*updatedValue)["setup"]["rounds"]["prompt"];
    EXPECT_EQ(updatedPrompt, "test changing the prompt");

    // Output the changed values for verification
    std::cout << "Audience is now: " << std::boolalpha << (*updatedValue)["audience"] << std::endl;
    std::cout << "Prompt is now: " << updatedPrompt << std::endl;
}

// Test for setting and getting configuration
TEST_F(DataBucketTest, HandleConfiguration) {
    nlohmann::json config = {{"gameName", "Chess"}, {"playerLimit", 2}};
    bucket.setConfiguration(config);

    auto retrievedConfig = bucket.getConfiguration();
    ASSERT_TRUE(retrievedConfig.has_value());
    EXPECT_EQ(retrievedConfig.value(), config);
}

// Test for setting and getting constants
TEST_F(DataBucketTest, HandleConstants) {
    nlohmann::json constants = {{"maxScore", 100}, {"bonusPoints", 20}};
    bucket.setConstants(constants);

    auto retrievedConstants = bucket.getConstants();
    ASSERT_TRUE(retrievedConstants.has_value());
    EXPECT_EQ(retrievedConstants.value(), constants);
}

// Test for setting and getting variables
TEST_F(DataBucketTest, HandleVariables) {
    nlohmann::json variables = {{"currentRound", 1}, {"isPaused", false}};
    bucket.setVariables(variables);

    auto retrievedVariables = bucket.getVariables();
    ASSERT_TRUE(retrievedVariables.has_value());
    EXPECT_EQ(retrievedVariables.value(), variables);
}

// Test for adding and retrieving a player's data
TEST_F(DataBucketTest, HandlePlayerData) {
    std::string playerId = "player123";
    PlayerBucket playerData = {3, "conn123"};

    bucket.addPlayer(playerId, playerData);

    auto retrievedPlayerData = bucket.getPlayerBucket(playerId);
    ASSERT_TRUE(retrievedPlayerData.has_value());
    EXPECT_EQ(retrievedPlayerData->wins, playerData.wins);
    EXPECT_EQ(retrievedPlayerData->connectionId, playerData.connectionId);
}

// Test for updating a player's data
TEST_F(DataBucketTest, UpdatePlayerData) {
    std::string playerId = "player123";
    PlayerBucket initialData = {3, "conn123"};
    bucket.addPlayer(playerId, initialData);

    PlayerBucket newData = {5, "conn456"};
    bucket.updatePlayerState(playerId, newData);

    auto updatedData = bucket.getPlayerBucket(playerId);
    ASSERT_TRUE(updatedData.has_value());
    EXPECT_EQ(updatedData->wins, newData.wins);
    EXPECT_EQ(updatedData->connectionId, newData.connectionId);
}

// Test for adding and retrieving an audience member's data
TEST_F(DataBucketTest, HandleAudienceData) {
    std::string audienceId = "audience123";
    AudienceBucket audienceData = {"conn789"};

    bucket.addAudienceMember(audienceId, audienceData);

    auto retrievedAudienceData = bucket.getAudienceBucket(audienceId);
    ASSERT_TRUE(retrievedAudienceData.has_value());
    EXPECT_EQ(retrievedAudienceData->connectionId, audienceData.connectionId);
}

// Test for updating an audience member's data
TEST_F(DataBucketTest, UpdateAudienceData) {
    std::string audienceId = "audience123";
    AudienceBucket initialData = {"conn789"};
    bucket.addAudienceMember(audienceId, initialData);

    AudienceBucket newData = {"conn987"};
    bucket.updateAudienceState(audienceId, newData);

    auto updatedData = bucket.getAudienceBucket(audienceId);
    ASSERT_TRUE(updatedData.has_value());
    EXPECT_EQ(updatedData->connectionId, newData.connectionId);
}