#pragma once

#include "RuleNode.h"


class ExecutionState {
public: // todo: control access to members, pass ScopedState by const ref?
    ExecutionState(RuleNode* rootNode, ScopedState initialState, ExecutionState* parent);

    [[nodiscard]] size_t getId() const;


    std::vector<std::unique_ptr<RuleNode>>::iterator it_start;
    std::vector<std::unique_ptr<RuleNode>>::iterator it_end;

    ScopedState localState;
    RuleNode* parentNode = nullptr;

    // variables for managing apparent parallelism and waiting
    bool shouldWait;
    ExecutionState* parentExecutionState;
    int numExecutingChildren;

private:
    // todo: test this functionality
    static size_t getNextExecutionStateId();

    size_t executionStateId; // unique ID for this ExecutionState instance
};

