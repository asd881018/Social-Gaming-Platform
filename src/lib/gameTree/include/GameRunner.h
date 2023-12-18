#pragma once

#include <queue>
#include "RuleNode.h"
#include "ExecutionState.h"
#include "DataBucket.h"
#include "Server.h"
#include "msgCreator.h"
#include "Expression.h"


// todo: temp, replace with proper type [only one element needed?  ImmutableKey?
typedef std::vector<std::pair<std::string, std::string>> Parameters;


class GameRunner {
public:
    explicit GameRunner(std::unique_ptr<RuleNode> rootNode, GameParams params);
    explicit GameRunner(); // todo

    void createAndAddExecutionState(RuleNode* startNode, const ScopedState& initialState, ExecutionState* parent);
    void updateParentOnChildCompletion(ExecutionState* parent);

    void processMessage(const jsonMsg& message);
    std::deque<jsonMsg> runGameUntilBlocked(const jsonMsg& messages);

    std::optional<sg::ExpressionValue> getIdentifierValueForExecutionState(const std::string& identifier, const ExecutionState& executionState);

    RuleNode* getRoot();

private:
    std::unique_ptr<RuleNode> root;

    // todo: rename / retype to capture notion of player state as well
//    DataBucket dataBucket; // todo: make not unique_ptr, reg w/move?
    GameParams gameParameters;


    // todo: needed? refactor?
    std::vector<std::unique_ptr<ExecutionState>> waitingList;
    std::queue<std::unique_ptr<ExecutionState>> readyQueue;

//    std::queue<networking::Message> messageQueue; // todo: needed?
};