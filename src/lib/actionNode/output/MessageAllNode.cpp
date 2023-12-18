#include "MessageAllNode.h"

#include <iostream>
#include <utility>

// MessageAllNode::MessageAllNode(std::string message, classifier::MessageClassifier* serverHandler)
//     : message(std::move(message)), serverHandler(serverHandler) {} // Store the messageClassifier pointer

DummyBucket MessageAllNode::execute(DummyBucket data) {
    requestForAllPlayers(data);
    data.setOk(true);
    return data;
}

void MessageAllNode::requestForAllPlayers(DummyBucket data) const {
    // NodeHelpers::sendToAllPlayers(*serverHandler, data, message); // Access sendToAllPlayers through the pointer
}
