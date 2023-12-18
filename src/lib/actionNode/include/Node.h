#pragma once

#include "NodeImpl.h"

#include <memory>
#include <utility>

class Node {
public:
    Node() = default;

    explicit Node(std::unique_ptr<NodeImpl>&& action) : action(std::move(action)) {}

    DummyBucket execute(DummyBucket data) {
        return action->execute(data);
    }

private:
    std::unique_ptr<NodeImpl> action;
};