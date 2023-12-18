#pragma once

#include "Node.h"
#include <chrono>
#include <optional>


class TimerNode : public NodeImpl {
public:
    explicit TimerNode(int wait);

    DummyBucket execute(DummyBucket data) override;

private:
    std::optional<std::chrono::high_resolution_clock::time_point> start;

    int wait;

    [[nodiscard]] bool waitTimePassed() const;
};
