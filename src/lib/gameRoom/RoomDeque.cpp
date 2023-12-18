#include "RoomDeque.h"

void RoomDeque::insert(std::deque<jsonMsg>& msg) {
    std::move(msg.begin(), msg.end(), std::back_inserter(messages));

}

void RoomDeque::insert(std::deque<jsonMsg>&& msg) {
    insert(msg);
}

void RoomDeque::insert(RoomDeque&& deque) {
    insert(deque.messages);
    if (deque.status == FINISHED) {this->status = FINISHED;}
}

void RoomDeque::insert(jsonMsg& jsonMsg) {
    messages.push_back(jsonMsg);
}
