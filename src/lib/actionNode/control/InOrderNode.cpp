#include <algorithm>
#include "InOrderNode.h"

DummyBucket InOrderNode::execute(DummyBucket data) {
    // marking nodes as complete by removing them from the list until more sophisticated methods are developed with the data bucket
    auto blockerIt = std::find_if(children.begin(), children.end(), [&data](auto& child) {
        data = child.execute(data);
        if (!data.isOk()) {
            return true;
        }
        return false;
    });

    children.erase(children.begin(), blockerIt);

    return data;
}

void InOrderNode::insert(Node& newChild) {
    this->children.emplace_back(std::move(newChild));
}
