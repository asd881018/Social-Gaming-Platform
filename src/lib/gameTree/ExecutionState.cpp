#include "ExecutionState.h"
#include <utility>

ExecutionState::ExecutionState(RuleNode* rootNode, ScopedState initialState, ExecutionState* parent)
    : localState(std::move(initialState)), parentNode(rootNode), parentExecutionState(parent) {

    executionStateId = getNextExecutionStateId();

    // todo: test this
    localState.execState = ExecState { executionStateId };

    it_start = rootNode->getChildren().begin();
    it_end = rootNode->getChildren().end();

    numExecutingChildren = 0;
    shouldWait = false;
}

size_t ExecutionState::getNextExecutionStateId() {
    static size_t s_executionStateId { 0 };
    return s_executionStateId++;
}

size_t ExecutionState::getId() const {
    return executionStateId;
}

