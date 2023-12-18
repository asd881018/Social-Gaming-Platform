#include "GameRunner.h"
#include <iostream>
#include <utility>


GameRunner::GameRunner(std::unique_ptr<RuleNode> rootNode, GameParams params)
    : root(std::move(rootNode)), gameParameters(std::move(params)) {

    ScopedState emptyState;
    readyQueue.push(std::make_unique<ExecutionState>(root.get(), emptyState, nullptr));
}

void GameRunner::createAndAddExecutionState(RuleNode* startNode, const ScopedState& initialState, ExecutionState* parent) {
    auto es = std::make_unique<ExecutionState>(startNode, initialState, parent);

    parent->numExecutingChildren++;
    parent->shouldWait = true;

    readyQueue.push(std::move(es));
}

void GameRunner::updateParentOnChildCompletion(ExecutionState* parent) {
    parent->numExecutingChildren--;

    if (parent->numExecutingChildren <= 0) {
        parent->shouldWait = false;

        auto itr = std::find_if(waitingList.begin(), waitingList.end(), [parent](auto& element) {
            return element.get() == parent;
        });

        if (itr != waitingList.end()) {
            readyQueue.push(std::move((*itr)));
            waitingList.erase(itr);
        }
    }
}

RuleNode* GameRunner::getRoot() {
    return root.get();
}

// todo: refactor using actual databucket impl
std::optional<sg::ExpressionValue> GameRunner::getIdentifierValueForExecutionState(const std::string& identifier, const ExecutionState& executionState) {
//    sg::ExpressionValue value {};

    // perform lookup in local state
    auto itr_local = executionState.localState.valueMap.map.find(Identifier {identifier});
    if(itr_local != executionState.localState.valueMap.map.end()) {
        std::cout << "Value found in local scope\n";
        return itr_local->second;
    }

    // identifier not found in scoped state, perform lookup in global state
    auto itr_global = gameParameters.globalState.map.find(Identifier {identifier});
    if(itr_global != gameParameters.globalState.map.end()) {
        std::cout << "Value found in global scope\n";
        return itr_global->second;
    }

    return std::nullopt;
}

void GameRunner::processMessage(const jsonMsg& message) {

    auto exState = message.execState;
    Identifier targetIdentifier = Identifier { message.target.id };
    auto messageContent = message.messageContent;

    // update the target identifier of the appropriate ExecutionState

    // find appropriate ExecutionState in waitingList
    auto itr = std::find_if(waitingList.begin(), waitingList.end(), [message](auto& listItem) {
        return listItem.get()->getId() == message.execState.id;
    });

    // find and update the appropriate Identifier's Value for this ExecutionState
    if (itr != waitingList.end()) {
        auto targetExecutionState = itr->get();

        targetExecutionState->localState.valueMap.map.insert(std::pair<Identifier, sg::ExpressionValue>(Identifier{"testID"}, sg::ExpressionValue{"testValue"}));
        // search localState, update if found
        auto itr_local = targetExecutionState->localState.valueMap.map.find(targetIdentifier);

        std::string testString = "testID";

        // todo: perform this search later, for efficiency
        auto itr_global = gameParameters.globalState.map.find(targetIdentifier);
        if (itr_local != targetExecutionState->localState.valueMap.map.end()) {
//        if (targetExecutionState->localState.valueMap.map.at(targetIdentifier) == ) {
            // todo: this is just strings for now.  how to handle more data types?
            itr_local->second = sg::ExpressionValue { messageContent };
        } else if (itr_global != gameParameters.globalState.map.end()) {
            // todo: this is just strings for now.  how to handle more data types?
            itr_global->second = sg::ExpressionValue { messageContent };
        } else {
            std::cout << "Error: Identifier not found in Scope for this ExecutionState\n";
        }

        // ready for processing again: by contract, a given ES can only have one outstanding message at a time
        readyQueue.push(std::move((*itr)));
        waitingList.erase(itr);
    } else {
        std::cout << "Error: target ExecutionState nor found in WaitingList\n";
    }
}

// Messages = deque
std::deque<jsonMsg> GameRunner::runGameUntilBlocked(const jsonMsg& inputMessage) {
    processMessage(inputMessage);

    std::unique_ptr<ExecutionState> currExState;
    std::deque<jsonMsg> outputMessages;

    // Main execution loop of game
    while (!readyQueue.empty() || !waitingList.empty()) {
        // Determine next ExecutionState to process
        if (readyQueue.empty()) {
            // Blocked: nothing to process, but still waiting on a request from the server to be fulfilled
            return outputMessages;
        }

        /* process next ExecutionState */
        currExState = std::move(readyQueue.front());
        readyQueue.pop();

        // Check if iterator has completed, and notify parent if so
        if (currExState->it_start >= currExState->it_end) {
            // done processing this set of children, notify parent ES
            auto& parent = currExState->parentExecutionState;

            // todo: message indicating 'game complete'? -> server?
            if(parent == nullptr) {
                // reached end of top level state, game has finished
                std::cout << "Done\n";
                return outputMessages;
            }

            updateParentOnChildCompletion(parent);
            continue;
        }

        // get next node to be processed from this ExecutionState
        auto& node = (*currExState->it_start);

        // Perform Control Flow Logic
        // todo: interpret expression here, and then use return msg?  need to parse expression BEFORE creating new ExStates
        if (node->getNodeType() == "parallel_for") {

            // todo: remove hardcoding
            int numLoops = 4;

            // todo: resolve variant demo
//            auto expression = &node->node_arguments.at(0).expr;
//
//            if (std::string str_value = std::get_if<std::string>(expression)) {
//                numLoops = stoi(str_value); // todo: temp, implement with actual params
//            }

            for (int i = 0; i < numLoops; i++) {
                // create a new State for the child execution branch, with a new var as needed
                ScopedState newState = currExState->localState;
//                newState.valueMap.map.insert(ImmutableKey{"player"}, std::to_string(i).c_str()); // todo: temp, something odd with String insertion into DataBucket?
                newState.valueMap.map.insert(std::make_pair<Identifier, sg::ExpressionValue>(Identifier{ "player" }, sg::ExpressionValue {"test"}));
                createAndAddExecutionState(node.get(), newState, currExState.get());
            }
        } // ... and other control flow type logic to follow
        // todo: timer node should send a message with a timeout to the server?


        // Execute Node - pass needed values to execute (ScopedState, global vars, others?)
        // todo: test this
        auto outMessages = node->execute(currExState.get(), gameParameters); // todo: best way to pass dataBucket -> pass both by ref instead for now

        if (outMessages.has_value()) {
            std::move(outMessages->begin(),
                      outMessages->end(),
                      std::back_inserter(outputMessages));
        }

        currExState->it_start++;


        // Add Execution State to appropriate container
        if (currExState->shouldWait) {
            // reset flag
            currExState->shouldWait = false;
            waitingList.push_back(std::move(currExState));
        } else {
            readyQueue.push(std::move(currExState));
        }

    }

    return outputMessages;
}

GameRunner::GameRunner() {

}
