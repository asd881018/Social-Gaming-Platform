#include "Node.h"

class InOrderNode : public NodeImpl {
public:
    DummyBucket execute(DummyBucket data) override;

    void insert(Node& newChild);

private:
    std::vector<Node> children;
};
