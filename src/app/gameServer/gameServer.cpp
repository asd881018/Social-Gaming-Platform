#include "Server.h"
#include "GameRoom.h"
#include "msgParser.h"
#include "msgCreator.h"
#include "MessageProcessor.h"
#include "Setup.h"
#include "RunningGame.h"
#include "GameInfo.h"
#include "GameScanner.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <unordered_map>

using networking::Connection;
using networking::Message;
using networking::Server;
using namespace messageProcessor;
using codeGenerator::RoomCodeGenerator;

//using jsonMsg;

inline const int RANGE_MIN = 100000;
inline const int RANGE_MAX = 999999;

// call back function for creating the server object
void onConnect(Connection c)
{
  std::cout << "New connection found: " << c.id << "\n";
}

// call back function for creating the server object
void onDisconnect(Connection c)
{
  std::cout << "Connection lost: " << c.id << "\n";
}

// grabs the html file used to create the server
// could get rid of this if we don't want the server hose to pass in their own html file
std::string getHTTPMessage(const char *htmlLocation)
{
  if (access(htmlLocation, R_OK) != -1)
  {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};
  }

  std::cerr << "Unable to open HTML index file:\n"
            << htmlLocation << "\n";
  std::exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  std::cout << "GameServer booted up!\n";

  // maps the gameRoom with gameRoom code as the key
  std::unordered_map<GameRoomID, GameRoom, GameRoomIDHash> gameRooms;

  // code generator for genering room codes and ensuring uniqueness of room codes
  RoomCodeGenerator roomCodeGenerator(RANGE_MIN, RANGE_MAX);

  // creates the server
  const unsigned short port = std::stoi(argv[1]);
  Server server = Server(port, getHTTPMessage(argv[2]), onConnect, onDisconnect);

  // main running loop of the server,
  while (true) {
    // recieves incoming messages from the server
    server.update();
    std::deque<Message> incomingMessages = server.receive();

    // transform the collection of incoming (networking) messages into a collection of jsonMsgs
    std::deque<jsonMsg> incomingJsonMessages = transformNetworkMsgs(incomingMessages);

    // We have decided that instead of batch processing we would do single message processing
    for (jsonMsg message : incomingJsonMessages) {
      std::deque<jsonMsg> outgoingJsonMessages = processMessages(message, gameRooms, roomCodeGenerator);

      // transforms the collection of jsonMsgs into networking messages so we can send them through the network
      std::deque<Message> outgoingMessages = transformJsonMsgs(outgoingJsonMessages);

      // sends out the messages returned by the processes as soon as we get them
      // this is done so that messages related to one game are send out and clients won't have to wait as long
      // we can do this because all games and their outgoing messages are independent of each other
      // if a reason comes up we can change it so it collects outgoing messages for all the processing and send it out all at once
      server.send(outgoingMessages);
    }
    // for development only so that it does not slow our devices while we are testing gameServer and other client
    // safe to remove when pushing to master
    sleep(1);
  }
}
