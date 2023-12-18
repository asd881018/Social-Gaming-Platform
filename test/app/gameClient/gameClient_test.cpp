
// #include <gtest/gtest.h>
// #include "../../../include/network/Client.h"

// // Define a mock Client class for testing purposes
// class MockClient : public networking::Client {
// public:
//     MockClient(const std::string& ipAddress, const std::string& port)
//         : networking::Client(ipAddress, port) {
//     }

//     // Override or extend methods as needed for testing

//     // Example: Simulate receiving a response from the server
//     void simulateReceiveResponse(const std::string& response) {
//         // Manually set the response for testing
//         setResponse(response);
//     }
// };

// TEST(GameClientTest, SendTextToServer) {
//     // Create a GameClient object using the MockClient (with test-specific IP and port)
//     MockClient mockClient("test_ip", "test_port");
//     GameClient gameClient(mockClient);

//     // Send a text to the server
//     gameClient.sendTextToServer("Hello, Server!");

//     // Add assertions to check if the sent text is as expected
//     // For example, we can check if the state is updated
//     ASSERT_EQ(gameClient.getState(), ExpectedStateAfterSend);
// }

// TEST(GameClientTest, ReceiveFromServer) {
//     // Create a GameClient object using the MockClient (with test-specific IP and port)
//     MockClient mockClient("test_ip", "test_port");
//     GameClient gameClient(mockClient);

//     // Simulate receiving a response from the server
//     mockClient.simulateReceiveResponse("This is a test response.");

//     // Check if the received response is as expected
//     ASSERT_EQ(gameClient.receiveFromServer(), "This is a test response.");
// }

// // Add more tests for different scenarios

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
