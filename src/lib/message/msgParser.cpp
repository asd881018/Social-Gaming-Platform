#include "msgParser.h"
#include <sstream>
using json = nlohmann::json;

#include <string>
#include <vector>

// converts a string with jsonmsg format back to a jsonmsg struct
jsonMsg parseJsonMessage(std::string message) {
    if (message.empty()) {
        return jsonMsg{};
    }
    json resultJson = json::parse(message);
    jsonMsg result = resultJson.template get<jsonMsg>();
    return result;
}

// converts one network msg to a json msg; sets the connection field of the jsonmsg
jsonMsg parseNetworkMsgToJsonMsg(const networking::Message &message) {
    json resultJson = json::parse(message.text);
    jsonMsg result = resultJson.template get<jsonMsg>();
    result.connection = message.connection;
    return result;
}

// transforms a collection of network msgs to a collection json msgs
std::deque<jsonMsg> transformNetworkMsgs(const std::deque<networking::Message> &messages) {
    std::deque<jsonMsg> jsonMessages;
    std::transform(messages.begin(), messages.end(), std::back_inserter(jsonMessages), parseNetworkMsgToJsonMsg);
    return jsonMessages;
}
