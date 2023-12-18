#ifndef MESSAGE_MESSAGEPARSER_H
#define MESSAGE_MESSAGEPARSER_H

#include "msgCreator.h"
#include "nlohmann/json.hpp"
#include "Server.h"
using json = nlohmann::json;

// converts a string with jsonmsg format back to a jsonmsg struct
jsonMsg parseJsonMessage(std::string message);

// converts one network msg to a json msg; sets the connection field of the jsonmsg
jsonMsg parseNetworkMsgToJsonMsg(const networking::Message &message);

// transforms a collection of network msgs to a collection json msgs
std::deque<jsonMsg> transformNetworkMsgs(const std::deque<networking::Message> &messages);

#endif