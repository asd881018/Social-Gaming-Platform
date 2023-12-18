
// #include <gtest/gtest.h>
// #include "../../../include/network/Server.h"

// // Define a mock Server class for testing purposes
// class MockServer : public networking::Server {
// public:
//     MockServer(unsigned short port, std::string httpMessage)
//         : networking::Server(port, httpMessage, onConnect, onDisconnect) {
//     }

//     // Override or extend methods as needed for testing

//     // Example: Simulate receiving a message from a client
//     void simulateReceiveMessage(const networking::Message& message) {
//         // Manually add the message for testing
//         receivedMessages.push_back(message);
//     }

//     // Example: Simulate disconnecting a client
//     void simulateDisconnectClient(const networking::Connection& connection) {
//         // Manually disconnect the client for testing
//         disconnectedClients.push_back(connection);
//     }

//     // Other methods or data to control behavior for testing
//     // You can extend this mock class as needed
// private:
//     static void onConnect(networking::Connection c) {
//         // Handle the connection as needed for testing
//     }

//     static void onDisconnect(networking::Connection c) {
//         // Handle the disconnection as needed for testing
//     }

//     std::deque<networking::Message> receivedMessages;
//     std::deque<networking::Connection> disconnectedClients;
// };

// TEST(GameServerTest, ReceiveMessages) {
//     // Create a GameServer object using the MockServer (with test-specific port and message)
//     MockServer mockServer(12345, "Test HTML Message");
//     GameServer gameServer(mockServer);

//     // Simulate receiving a message from a client
//     networking::Connection clientConnection{1};
//     networking::Message message{clientConnection, "Test message from client"};
//     mockServer.simulateReceiveMessage(message);

//     // Call the function to process received messages
//     gameServer.processReceivedMessages();

//     // Check if the expected processing occurred
//     // For example, check if the response was generated
//     // and if it was added to the outgoing messages
//     ASSERT_TRUE(/* Check expected results */);
// }

// TEST(GameServerTest, DisconnectClient) {
//     // Create a GameServer object using the MockServer (with test-specific port and message)
//     MockServer mockServer(12345, "Test HTML Message");
//     GameServer gameServer(mockServer);

//     // Simulate disconnecting a client
//     networking::Connection clientConnection{2};
//     mockServer.simulateDisconnectClient(clientConnection);

//     // Call the function to process disconnecting clients
//     gameServer.processDisconnectedClients();

//     // Check if the client was removed from the game server's data structures
//     ASSERT_TRUE(/* Check if the client was removed */);
// }

// // You can add more tests for different scenarios

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
