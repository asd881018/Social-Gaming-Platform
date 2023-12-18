#pragma once

#include <string>

#include "NodeImpl.h"
#include "TimerNode.h"
#include "NodeHelpers.h"

class InputTextNode : public NodeImpl {
public:
    // InputTextNode(const std::pair<std::string, std::string>& target, std::string prompt,
    //               std::string player, classifier::MessageClassifier* serverHandler);

    // InputTextNode(const std::pair<std::string, std::string>& target, std::string prompt,
    //               std::string player, classifier::MessageClassifier* serverHandler, TimerNode& timeout);

    DummyBucket execute(DummyBucket data) override;

private:
    // waiting on further implementation and design to get these member variables more concrete classes
    std::pair<std::string, std::string> target;
    std::string prompt;
    std::string player;

    std::optional<TimerNode> timeout{std::nullopt};
    bool requested = false;

    bool receivedValue(DummyBucket data);

    void requestFromServer(DummyBucket data) const;
};
