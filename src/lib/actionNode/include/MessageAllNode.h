#pragma once

#include "Node.h"
#include "NodeHelpers.h"

#include <string>
#include <vector>

class MessageAllNode : public NodeImpl {
public:
    // explicit MessageAllNode(std::string message, classifier::MessageClassifier* serverHandler); // Accept a pointer to MessageClassifier

    DummyBucket execute(DummyBucket data) override;

private:
    std::string message;
    void requestForAllPlayers(DummyBucket data) const; // No need to pass messageClassifier here
};
