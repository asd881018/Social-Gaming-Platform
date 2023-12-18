#include "InputTextNode.h"

#include <iostream>
#include <utility>

// InputTextNode::InputTextNode(const std::pair<std::string, std::string>& target, std::string prompt,
//                              std::string player, classifier::MessageClassifier* serverHandler)
//     : serverHandler(serverHandler), target(target), prompt(std::move(prompt)), player(std::move(player)) {}

// InputTextNode::InputTextNode(const std::pair<std::string, std::string>& target, std::string prompt,
//                              std::string player, classifier::MessageClassifier* serverHandler, TimerNode& timeout)
//     : serverHandler(serverHandler), target(target), prompt(std::move(prompt)), player(std::move(player)) {
//     this->timeout = timeout;
// }

DummyBucket InputTextNode::execute(DummyBucket data) {
    if (requested) {
        if (receivedValue(data) ||
            (timeout.has_value() && timeout.value().execute(data).isOk())) {
            data.setOk(true);
            return data;
        }
        data.setOk(false);
        return data;
    }

    requestFromServer(data);
    if (timeout.has_value()) { timeout->execute(data); }

    requested = true;
    data.setOk(false);
    return data;
}

// stub, should send a request to the server for user input using data from the databucket for player information and variables
void InputTextNode::requestFromServer(DummyBucket data) const {

    int dummyIndex = 1;
    // Send a request to the server for user input
    // NodeHelpers::sendUserInputRequest(*serverHandler, data, dummyIndex);
}

// stub, should check game bucket to see if given target has been updated by server yet
bool InputTextNode::receivedValue(DummyBucket data) {
    // Check the game bucket or relevant data source to see if the target has been updated
    // We should implement the logic to check if the target is updated
    return data.isTargetUpdated(target); // Implement this method as needed
}