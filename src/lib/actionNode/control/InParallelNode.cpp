#include "InParallelNode.h"
#include <algorithm>


DummyBucket InParallelNode::execute(DummyBucket data) {
    // to be replaced with a more elaborate node marking structure using data bucket
    auto blockerIt = std::stable_partition(children.begin(), children.end(), [&data](auto& child) {
        // will need some sort of scoping mechanism in the future that creates individual data buckets for each operation and somehow combines them together at the end
        data = child.execute(data);
        if (!data.isOk()) {
            return true;
        }
        return false;
    });

    children.erase(blockerIt, children.end());

    if (children.empty()) {
        return DummyBucket(true);
    }
    return DummyBucket(false);
}

void InParallelNode::insert(Node& child) {
    this->children.emplace_back(std::move(child));
}

