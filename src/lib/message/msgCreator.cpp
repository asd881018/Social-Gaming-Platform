#include "msgCreator.h"
#include "nlohmann/json.hpp"
#include <algorithm>
using json = nlohmann::json;

std::string createJsonMessage(jsonMsg msg) {
    json result = msg;
    std::string resultStr = result.dump();
    return resultStr;
}

// converts one json msg to a network msg; sets the connection id field of the jsonmsg
networking::Message parseJsonMsgToNetworkMsg(const jsonMsg& message) {
    std::string messageText = createJsonMessage(message);
    return networking::Message{message.connection, messageText};
}

// transforms a collection of json msgs to a collection of networking msgs
std::deque<networking::Message> transformJsonMsgs(const std::deque<jsonMsg> &messages) {
    std::deque<networking::Message> networkingMessages;
    // I use a lamda function instead of just using the function pointer, due to my compiler having some issue 
    // understanding the arguments when I just use the function pointer
    std::transform(messages.begin(), messages.end(), std::back_inserter(networkingMessages), [] (const jsonMsg& message) {
        return parseJsonMsgToNetworkMsg(message);
    });
    return networkingMessages;
}
