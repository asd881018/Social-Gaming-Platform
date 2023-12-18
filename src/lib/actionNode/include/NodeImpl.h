# pragma once

#include "DummyBucket.h"

class NodeImpl {
public:
    virtual DummyBucket execute(DummyBucket data) = 0;

    virtual ~NodeImpl() = default;
};

