#ifndef NEWCLIENT_H
#define NEWCLIENT_H

#include <iostream>
#include <string>
#include "Client.h"
#include "msgCreator.h"
#include "msgParser.h"


std::string getUsername();
std::string getCreateOrJoinChoice();
int getRoomCode();
std::string getUserInputInRange(int minRange, int maxRange);
std::string receiveNonEmptyResponse(networking::Client& client);
void runClient(networking::Client& client);
int main(int argc, char* argv[]);

#endif // NEWCLIENT_H