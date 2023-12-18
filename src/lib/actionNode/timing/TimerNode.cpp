#include "TimerNode.h"
#include <chrono>

using namespace std::chrono;

TimerNode::TimerNode(int wait) : start{std::nullopt}, wait(wait) {}

DummyBucket TimerNode::execute(DummyBucket data) {
    if (waitTimePassed()) {
        data.setOk(true);
        return data;
    }

    if (!this->start.has_value()) {
        this->start = high_resolution_clock::now();
    }

    data.setOk(false);
    return data;
}

bool TimerNode::waitTimePassed() const {
    if (start.has_value() &&
        duration_cast<seconds>(high_resolution_clock::now() - this->start.value()).count() > this->wait) {
        return true;
    }
    return false;
}



