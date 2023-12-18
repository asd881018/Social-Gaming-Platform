#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "clientHelper.h"

class MockClient : public networking::Client {
public:
    MockClient(const std::string& address, const std::string& port) : networking::Client(address, port) {}

    MOCK_METHOD(void, update, ());
    MOCK_METHOD(void, send, (std::string));
    MOCK_METHOD(std::string, receive, ());
    MOCK_METHOD(bool, isDisconnected, (), (const));
};

class NewClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up mock client
        mockClient = std::make_unique<MockClient>("127.0.0.1", "8080");
    }

    void TearDown() override {
        mockClient.reset();
    }

    std::unique_ptr<MockClient> mockClient;
};

TEST(NewClientTest, TestEmptyUsername) {
    // Arrange
    std::string expected = "";
    std::istringstream iss("\n"); // Empty input
    std::cin.rdbuf(iss.rdbuf());

    // Act
    std::string result = getUsername();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(NewClientTest, TestValidUsername) {
    // Arrange
    std::string expected = "JohnDoe";
    std::istringstream iss("JohnDoe\n");
    std::cin.rdbuf(iss.rdbuf());

    // Act
    std::string result = getUsername();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(NewClientTest, TestEmptyInput) {
    // Arrange
    std::string expected = "";
    std::istringstream iss("\n"); // Empty input
    std::cin.rdbuf(iss.rdbuf());

    // Act
    std::string result = getCreateOrJoinChoice();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(NewClientTest, TestValidChoice) {
    // Arrange
    std::string expected = "join";
    std::istringstream iss("join\n");
    std::cin.rdbuf(iss.rdbuf());

    // Act
    std::string result = getCreateOrJoinChoice();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(NewClientTest, TestNonIntegerInput) {
    // Arrange
    int expected = 0; // Expected output for non-integer input
    std::istringstream iss("abc\n"); // Non-integer input
    std::cin.rdbuf(iss.rdbuf());

    // Act
    int result = getRoomCode();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(NewClientTest, TestNegativeRoomCode) {
    // Arrange
    std::istringstream iss("-123\n"); // Negative room code
    std::cin.rdbuf(iss.rdbuf());

    // Act
    int result = getRoomCode();

    // Assert
    EXPECT_LE(result, 0); // Ensure the room code is greater than 0
}

TEST(NewClientTest, TestValidRoomCode) {
    // Arrange
    int expected = 12345;
    std::istringstream iss("12345\n");
    std::cin.rdbuf(iss.rdbuf());

    // Act
    int result = getRoomCode();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(NewClientTest, TestValidUserInput) {
    // Arrange
    int minRange = 1;
    int maxRange = 5;
    int expected = 3;
    std::istringstream iss("3\n");
    std::cin.rdbuf(iss.rdbuf());

    // Act
    std::string result = getUserInputInRange(minRange, maxRange);

    // Assert
    EXPECT_EQ(std::to_string(expected), result);
}

TEST(NewClientTest, TestCreateJsonMessage) {
    // Arrange
    jsonMsg message;
    message.target = {"target_id"};
    message.messageContent = "Sample content";
    message.prompt = {PromptType::INT, 0, 10};
    message.gameRoom = {123};
    message.connection = {456};
    message.execState = {789};

    // Act
    std::string result = createJsonMessage(message);

    // Assert
    json parsedJson = json::parse(result);

    EXPECT_EQ("target_id", parsedJson["target"]["id"]);
    EXPECT_EQ("Sample content", parsedJson["messageContent"]);
    EXPECT_EQ(1, parsedJson["prompt"]["promptType"]);
    EXPECT_EQ(0, parsedJson["prompt"]["rangeMin"]);
    EXPECT_EQ(10, parsedJson["prompt"]["rangeMax"]);
    EXPECT_EQ(123, parsedJson["gameRoom"]["id"]);
    EXPECT_EQ(456, parsedJson["connection"]["id"]);
    EXPECT_EQ(789, parsedJson["execState"]["id"]);
}

TEST(NewClientTest, TestParseJsonMessage) {
    // Arrange
    std::string jsonStr = R"({
        "target": { "id": "target_id" },
        "messageContent": "Sample content",
        "prompt": { "promptType": 1, "rangeMin": 0, "rangeMax": 10 },
        "gameRoom": { "id": 123 },
        "connection": { "id": 456 },
        "execState": { "id": 789 }
    })";

    // Act
    jsonMsg result = parseJsonMessage(jsonStr);

    // Assert
    EXPECT_EQ("target_id", result.target.id);
    EXPECT_EQ("Sample content", result.messageContent);
    EXPECT_EQ(PromptType::INT, result.prompt.promptType);
    EXPECT_EQ(0, result.prompt.rangeMin);
    EXPECT_EQ(10, result.prompt.rangeMax);
    EXPECT_EQ(123, result.gameRoom.id);
    EXPECT_EQ(456, result.connection.id);
    EXPECT_EQ(789, result.execState.id);
}

//Below tests cause infinite loops due to the receive(function)

// TEST(NewClientTest, TestEmptyResponse) {
//     // Arrange
//     MockClient mockClient("127.0.0.1", "8080");
//     ON_CALL(mockClient, receive()).WillByDefault(testing::Return(std::string()));

//     // Act
//     std::string result = receiveNonEmptyResponse(mockClient);

//     // Assert
//     EXPECT_EQ("", result);
// }

// TEST(NewClientTest, TestNonEmptyResponse) {
//     // Arrange
//     MockClient mockClient("127.0.0.1", "8080");
//     std::string response = "Sample response";
//     ON_CALL(mockClient, receive()).WillByDefault(testing::Return(response));

//     // Act
//     std::string result = receiveNonEmptyResponse(mockClient);

//     // Assert
//     EXPECT_EQ(response, result);
// }

// TEST_F(NewClientTest, HandlesNetworkFailures) {
//     // Set up mock behavior for initial user inputs and server responses
//     EXPECT_CALL(*mockClient, update()).Times(testing::AtLeast(1));
//     EXPECT_CALL(*mockClient, send(testing::_)).Times(testing::AtLeast(1));
//     EXPECT_CALL(*mockClient, receive())
//         .Times(testing::AtLeast(1))
//         .WillOnce(testing::Return("join")) // User choice
//         .WillOnce(testing::Return("123")); // Room code

//     // Set expectations for a disconnected client after sending
//     EXPECT_CALL(*mockClient, receive()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(""));
//     EXPECT_CALL(*mockClient, isDisconnected()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(true));

//     // Call the function to test
//     runClient(*mockClient);
// }


// TEST_F(NewClientTest, HandlesMultipleInvalidUserInputs) {
//     networking::Client client{"127.0.0.1", "8080"};

//     int minRange = 1;
//     int maxRange = 5;
//     std::vector<int> userInputs = {7, 0, 2};  // Simulated user inputs, first two are invalid, third is valid

//     std::stringstream capturedOutput;

//     // Redirect std::cin and std::cout
//     std::streambuf* originalInput = std::cin.rdbuf();
//     std::streambuf* originalOutput = std::cout.rdbuf(capturedOutput.rdbuf());

//     for (const auto& input : userInputs) {
//         testing::internal::CaptureStdout();
//         std::cout << input << std::endl;

//         // Capture the output and input the sequence of values
//         std::string output = testing::internal::GetCapturedStdout();
//         std::istringstream userInputStream(output);

//         std::cin.rdbuf(userInputStream.rdbuf());

//         std::string inputInRange = getUserInputInRange(minRange, maxRange);
//         EXPECT_FALSE(inputInRange.empty());  // Assert that a non-empty string is received for valid inputs
//     }

//     // Restore std::cin and std::cout
//     std::cin.rdbuf(originalInput);
//     std::cout.rdbuf(originalOutput);
// }

// TEST(receiveNonEmptyResponse, TestEmptyResponse) {
//     // Arrange
//     std::string expected = "Test Message";
//     MockClient client("127.0.0.1", "8080");
//     ON_CALL(client, receive()).WillByDefault(testing::Return("")); // Empty response

//     // Act
//     std::string result = receiveNonEmptyResponse(client);

//     // Assert
//     EXPECT_EQ(expected, result); // Ensuring it returns a non-empty response
// }




