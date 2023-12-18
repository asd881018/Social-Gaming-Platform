#include <vector>

#include "Node.h"

class InParallelNode : public NodeImpl {
public:
    DummyBucket execute(DummyBucket data) override;

    void insert(Node& child);

private:
    std::vector<Node> children;
};
