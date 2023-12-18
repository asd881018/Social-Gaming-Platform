#include "clientHelper.h"

// Function to get user's username
std::string getUsername() {
    std::string username;
    std::cout << "Enter your username: ";
    std::cin >> username;
    return username;
}

// Function to get user's choice to create or join a game room
std::string getCreateOrJoinChoice() {
    std::string input;
    std::cout << "Enter 'create' to create a game room or 'join' to join one: ";
    std::cin >> input;
    return input;
}

// Function to get the room code if joining a room
int getRoomCode() {
    int code;
    std::cout << "Please enter the room code: ";
    std::cin >> code;
    return code;
}

// Function to get user input within a specified range
std::string getUserInputInRange(int minRange, int maxRange) {
    int userInput;
    do {
        std::cout << "Choose an option from " << minRange << " to " << maxRange << ": ";
        std::cin >> userInput;
    } while (userInput < minRange || userInput > maxRange);

    return std::to_string(userInput);
}

std::string receiveNonEmptyResponse(networking::Client& client) {
    std::string response;
    do {
        client.update();
        response = client.receive();
        //std::cout << "updating" << std::endl;
    } while (response.empty());
    return response;
}

// Function to handle the main client operations
void runClient(networking::Client& client) {
    client.update();
    
    //networking::Client client{ipAddress, port};
    bool flag = false;

    std::string username = getUsername();
    std::string choice = getCreateOrJoinChoice();
    int roomCode = 0;

    jsonMsg message;
    jsonMsg responseMessage;

    do {
        if (choice == "join") {
            roomCode = getRoomCode();
            message.gameRoom = {roomCode};
        } else {
            message.gameRoom = {0};
        }

        std::string messageStr = createJsonMessage(message);

        client.send(messageStr);

        responseMessage = parseJsonMessage(receiveNonEmptyResponse(client));
        std::cout << responseMessage.messageContent << std::endl;
    } while (responseMessage.gameRoom.id == -1);

    roomCode = responseMessage.gameRoom.id;

    while (!flag && !client.isDisconnected()) {
        jsonMsg message;
        
        responseMessage = parseJsonMessage(receiveNonEmptyResponse(client));

        std::cout << responseMessage.messageContent; 

        if (responseMessage.prompt.rangeMax != 0) {
            message.messageContent = getUserInputInRange(responseMessage.prompt.rangeMin, responseMessage.prompt.rangeMax);
        }

        std::string messageStr = createJsonMessage(message);
        client.send(messageStr);
    }
}