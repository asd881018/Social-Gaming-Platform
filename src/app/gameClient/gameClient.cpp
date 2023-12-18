#include <unistd.h>
#include <vector>

#include "Client.h"
#include "msgCreator.h"
#include "msgParser.h"
#include "GameRoom.h"
#include <iostream>
#include <utility>
#include <vector>

// Made these variables global just so I could get this function working
// TODO: Put these global variables and the following 2 functions is some sort of controller class or however we decide to do it
// Do the same with the big switch
bool done = false;
std::string response;
inline constexpr std::string_view quitStr = "quit";
// sends text to the server 
// if quit or exit passed it it will shut down. That should be in a different function
void sendToServer(networking::Client &client, std::string text) {
    // TODO: Figure out a way to wait for user input without halting the execution of the program

  if (quitStr == text) {
        done = true;
  } else {
    std::cout << "Sending " << text << " to server!" << std::endl;
    client.send(text);
  }
}

// grabs the response from the client
// ideally we will have class for this with response and client as variables
void receiveFromServer (networking::Client &client) {
  // stores the string from the server
  response = client.receive();
}


int
main(int argc, char* argv[]) {
    // error checking to see if the bare min arguments were passed it
    // prints the proper format that the user should have used
  if (argc < 3) {
    std::cerr << "Usage : \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 4002\n";
    return 1;
  }

  // creates a client with the ip address and the port
  networking::Client client{argv[1], argv[2]};

  // MAY BE USED IF WE DECIDE TO USE THE ChatWindow API TO CREATE OUR UI 
  // this is a call back function passed to the chatWindow 
  // when it recieves a string it will send the string to the client
  // or close the client if the string is a keyword
  // auto onTextEntry = [&done, &client] (std::string text) {
  //   if ("exit" == text || "quit" == text) {
  //     done = true;
  //   } else {
  //     client.send(text);
  //   }
  // };
  // creates a chat window i.e. a class that handles the UI
  // ChatWindow chatWindow(onTextEntry);

  // int clientState = GameState::TITLE_SCREEN;
  std::cout << "Welcome to the <SOCIAL GAMING PROJECT>" << '\n'; 
/*
//---JSON STRING EXAMPLE---
//Converting jsonMsg struct to json
         Target target = {"target!"};
         Prompt myPrompt = {PromptType::STRING, 0, 10};
         GameRoomID gameRoomID = {100132};
         networking::Connection connection = {18};
         ExecState state = {1};
         jsonMsg newMsg= {target, "Test Message!", myPrompt, gameRoomID, connection, state};
         json newJsonMsg = createJsonMessage(newMsg);
         std::cout << "json message is: " << std::endl << std::setw(4) << newJsonMsg << std::endl;

//Converting json to jsonMsg struct
         jsonMsg parsed = parseJsonMessage(newJsonMsg);
         std::cout << "parsed prompt type is: " << parsed.prompt.promptType << std::endl;
         std::cout << "parsed messageContent is: " << parsed.messageContent << std::endl;

//---GameRoom unordered_map Example---
  std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> gameRoomMap;
  networking::Connection ownerConnection = networking::Connection{1234};
      // creates a new game room in the set up state
  GameScanner gameManager;
  auto setUp = std::make_unique<roomState::Setup>(gameManager.viewGames());
  GameRoomID id1 = {112314};
  //GameRoom gameRoom1 = GameRoom(ownerConnection, id1, std::move(setUp)); 
  gameRoomMap.emplace(id1, GameRoom(ownerConnection, id1, std::move(setUp)));
  auto got = gameRoomMap.find(id1);
  std::cout << "GameRoomID is: " << (got->first).id << std::endl;
  */
  
  // main loop for sending and recieving messages to/from the serve over the network
  while (!done && !client.isDisconnected()) {

    // TO BE REPLACED VERY SOON
  //   try {
  //     client.update();
  //   } catch (std::exception& e) {
  //     std::cout << "Exception from Client update: " << e.what();
  //     done = true;
  //   }

  //   // TODO: create parser for the different types of responses we can expect from the server for each state
  //     // TODO: create a output to the client screen depending on the state of the client as well as the 
  //     // TODO: create a controller class that we can put this swithch into if we decide to use something like this
  //     switch (clientState)
  //     {
  //     case GameState::TITLE_SCREEN:
  //     {
  //       int input = 0;
  //       std::cout << "\nEnter 1 to Create Lobby\n";
  //       std::cout << "Enter 2 to Join Lobby\n";
  //       std::cin >> input;
  //       switch (input)
  //       {
  //       case 1:
  //         clientState = GameState::LOBBY_CREATION;
  //         break;
  //       case 2:
  //         clientState = GameState::LOBBY_JOIN;
  //         break;
  //       default:
  //         std::cout << "Invalid Input";
  //         break;
  //       }

  //       break;
  //     }
  //     case GameState::LOBBY_CREATION:
  //     {
  //       std::cout << "\nWelcome to Lobby Creation" << '\n';
  //       std::string playerName;
  //       std::string msg;
  //       int numPlayers;
  //       std::string creationMsg;
  //       // Prompts user for configuration for the game room
        
  //       std::cout << "Please enter your name: ";
  //       std::cin >> playerName;
  //       std::cout << "How many people in your party? (Integer): ";
  //       std::cin >> numPlayers;
  //       msg = playerName + "," + std::to_string(numPlayers);
  //       // creationMsg = createJsonMessage();
  //       creationMsg += ", gameState 1 ";

  //       sendToServer(client, creationMsg);
  //       clientState = GameState::LOADING;
  //       break;
  //     }
  //     case GameState::LOBBY_JOIN:
  //     {
  //       std::string code;
  //       std::cout << "\nPlease enter the Lobby Room Code:" << '\n';
  //       std::cin >> code;
  //       sendToServer(client, code);
  //       // TODO: Figure out the format we want the strings in so that the server and webclient
  //       // can parse each others string
  //       // Maybe just json
  //       // Maybe have a class function or method where we can do something like this
  //       // class.sendCode(code);
  //       // it would parse the code into a format the server could understand and send it for us        
  //       clientState = GameState::LOADING;
  //       break;
  //     }
  //     case GameState::LOBBY:
  //       break;
  //     // case GameState::IN_GAME:
  //     //   break;
  //     case GameState::LOADING:
  //     {
  //       // when we are in the loading screen
  //       // we wait for a response from the server
  //       // the server tells us what the next state to display
  //       // for example after they putin a room code
  //       // wait for the response to know if they enter a lobby or if they entered an invalid code
  //       receiveFromServer(client);
  //       if (!response.empty()) {
  //        nextState = response;
  //         // if the response is not empty we will want to parse the string and figure out what we want to do with it
  //         // depending on the the state we will expect a different response from the server
  //         // and how we will parse the input will be different also what we output to the screen will also be different
  //       std::string codeMsg = parseJsonMessage(nextState);
  //       std::cout << "codeMsg is: " << codeMsg << '\n';

  //       // sendToServer(client, codeMsg); 
  //       break;
  //       }

      
  //       break;
  //     }
  //     default:
  //     {
  //       std::cout << "Client in Broken State";
  //       done = true;
  //       break;
  //     }
  //     }
  }

  return 0;
}