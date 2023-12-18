#include <stack>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include "RuleNode.h"
#include "NodeActions.h"

// todo: make node_types explicit, not strings

RuleNode::RuleNode(std::string type, std::vector<sg::ExpressionValue> arguments) : nodeType(std::move(type)),
                                                                                   nodeArguments(std::move(arguments)) {
    nodeId = 0; // todo: implement? needed?
}

void RuleNode::addChild(const std::string& type, const Arguments& arguments) {
    this->addChild(std::make_unique<RuleNode>(type, arguments));
}

void RuleNode::addChild(std::unique_ptr<RuleNode> newChildNode) {
    auto& newNode = this->children.emplace_back(std::move(newChildNode));
    newNode->parent = this;
}


uint32_t RuleNode::getNodeId() const {
    return nodeId;
}

RuleNode* RuleNode::getParent() {
    return parent;
}

std::string RuleNode::getNodeType() const {
    return nodeType;
}

std::vector<sg::ExpressionValue> RuleNode::getNodeArguments() const {
    return nodeArguments;
}

std::unique_ptr<RuleNode>& RuleNode::getChild(int index) {
    return children[index];
}

std::vector<std::unique_ptr<RuleNode>>& RuleNode::getChildren() {
    return children;
}


void RuleNode::print() {
    std::stack<RuleNode*> stack;

    // contains all visited nodes in preorder traversal
    std::vector<std::string> preorder;

    stack.push(this);

    while (!stack.empty()) {
        RuleNode* temp = stack.top();
        stack.pop();

        preorder.push_back(temp->nodeType);

        // push all children onto stack from right to left todo: improve, rev iterator
        for (int i = int(temp->children.size() - 1); i >= 0; i--) {
            stack.push(temp->children[i].get());
        }
    }

    for (const auto& str: preorder) {
        std::cout << str << "  ";
    }
}

// todo: state updating - return, or ref?
std::optional<std::vector<jsonMsg>> RuleNode::execute(ExecutionState* exState, GameParams& params) {
    std::optional<std::vector<jsonMsg>> messages;

    // todo: test.  way to make this work without storing pointers?
    gameParameters = &params;
//    localState = &state;
    executionState = exState;

    auto it = executerMap.find(nodeType);
    if (it != executerMap.end()) {
        // todo: handle return
        messages = it->second(this);

    } else {
        std::cout << "No classifier found for node type: " << nodeType << std::endl;
    }

    return messages;
}

void printTree(RuleNode* root) {
    root->print();
}

// Todo: make printTree work
void printTree(std::unique_ptr<RuleNode>& root) {
//    root->print();
    std::function<void(RuleNode*)> printNode = [](RuleNode* node) {
        std::cout << node->getNodeType() << "  ";
    };

    std::function<void(RuleNode*)> printChildren = [&printNode](RuleNode* node) {
        for (auto& child: node->getChildren()) {
            printNode(child.get());
        }
    };

    std::function<void(RuleNode*)> printTree = [&printNode, &printChildren, &printTree](RuleNode* node) {
        printNode(node);
        printChildren(node);
        for (auto& child: node->getChildren()) {
            printTree(child.get());
        }
    };

    printTree(root.get());
}

/** ------------------ NodeExecuter Methods------------------ */
// todo: use actual types
std::map<std::string, RuleNode::NodeExecuter> RuleNode::executerMap = {
        {"message",      &RuleNode::execute_message},
        {"parallel_for", &RuleNode::execute_parallel_for},
        {"for",          &RuleNode::execute_for},
        {"loop",         &RuleNode::execute_loop},
        {"in_parallel",  &RuleNode::execute_in_parallel},
        {"match",        &RuleNode::execute_match},
        {"extend",       &RuleNode::execute_extend},
        {"reverse",      &RuleNode::execute_reverse},
        {"sort",         &RuleNode::execute_sort},
        {"deal",         &RuleNode::execute_deal},
        {"discard",      &RuleNode::execute_discard},
        {"assignment",   &RuleNode::execute_assignment},
        {"timer",        &RuleNode::execute_timer},
        {"input_choice", &RuleNode::execute_input_choice},
        {"input_text",   &RuleNode::execute_input_text},
        {"input_vote",   &RuleNode::execute_input_vote},
        {"input_range",  &RuleNode::execute_input_range},
        {"scores",       &RuleNode::execute_scores}
};

std::optional<std::vector<jsonMsg>> RuleNode::execute_message() {
    // args:
    // [player_set] (can accept 'all' as shorthand for 'all players')
    // [quoted_string] message
    // NOTE: currently only support for 'all'

    std::vector<jsonMsg> messages;

    // todo: expand error checking, make robust and more informative
    assert(nodeArguments.size() == 2 && "Incorrect number of arguments");

    std::vector<networking::Connection> player_list;
    const std::string* arg0String = std::get_if<std::string>(&nodeArguments[0].expressionValue);
//    const std::vector<sg::ExpressionValue>* arg0VectorString = std::get_if<std::vector<sg::ExpressionValue>>(&nodeArguments[0].expressionValue);

    if (arg0String) {
        assert(*arg0String == "all" && "Unknown argument");
        // message should be sent to every player
        player_list = gameParameters->players;
    } else {
        // todo: evaluate expression as a list of players.  Pass as std::string and convert to uintptr_t to support size of networking::Connection?
        std::cout << "Error: unsupported operation.  Could not evaluate Expression as list of players.\n";
    }


    const std::string* arg1String = std::get_if<std::string>(&nodeArguments[1].expressionValue);
    if (arg1String) {
        for (auto player: player_list) {
            std::cout << "message to [" << player.id << "]: " << *arg1String << ", ExecState: "
                      << executionState->localState.execState.id << "\n";
            "\n";

            jsonMsg newMessage;
            newMessage.messageContent = *arg1String;
            newMessage.gameRoom = {gameParameters->gameRoomId};
            newMessage.connection = player;
            newMessage.execState = {executionState->localState.execState};

            messages.push_back(newMessage);
        }
    } else {
        std::cout << "Error: could not evaluate message\n";
    }

    return std::make_optional<std::vector<jsonMsg>>(messages);
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_parallel_for() {
    std::cout << "This is a parallel_for node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_for() {
    std::cout << "This is a for node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_loop() {
    std::cout << "This is a loop node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_in_parallel() {
    std::cout << "This is an in_parallel node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_match() {
    std::cout << "This is a match node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_extend() {
    std::cout << "This is an extend node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_reverse() {
    std::cout << "This is a reverse node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_sort() {
    std::cout << "This is a sort node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_deal() {
    std::cout << "This is a deal node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_discard() {
    std::cout << "This is a discard node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_assignment() {
    std::cout << "This is an assignment node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_timer() {
    std::cout << "This is a timer node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_input_choice() {
    std::cout << "This is an input_choice node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_input_text() {
    // [qualified_identifier] target player (Player)
    // [quoted_string] prompt
    // [identifier] target named variable in which to store result
    // [optional] timeout (NOTE: plan for impl in future iteration)

    std::vector<jsonMsg> returnMessages;
    networking::Connection player{};

    assert(nodeArguments.size() >= 3 && nodeArguments.size() <= 4 && "Incorrect number of arguments");

    // todo: make identifier:  DANGER with int -> uintptr_t conversion as-is.  How to resolve? [Store identifier->player?]
    const int* arg0IntTargetPlayer = std::get_if<int>(&nodeArguments[0].expressionValue);
    const std::string* arg1StringPrompt = std::get_if<std::string>(&nodeArguments[1].expressionValue);

    // todo: make identifier instead
    const std::string* arg2StringTargetIdentifier = std::get_if<std::string>(&nodeArguments[2].expressionValue);


    if (arg0IntTargetPlayer && arg1StringPrompt && arg2StringTargetIdentifier) {
        // todo: validate
        player = networking::Connection{
                static_cast<uintptr_t>(*arg0IntTargetPlayer)}; // todo: remove this dangerous cast
        std::cout << "prompt to [" << player.id << "]: " << *arg1StringPrompt << ", to be stored in Identifier: "
                  << *arg2StringTargetIdentifier << ", from ExecState: " << executionState->localState.execState.id
                  << ", RoomID: " << gameParameters->gameRoomId.id << "\n";

        jsonMsg newMessage;
        newMessage.target = Target{*arg2StringTargetIdentifier};
        newMessage.messageContent = *arg1StringPrompt;
        newMessage.gameRoom = {gameParameters->gameRoomId};
        newMessage.connection = player;

        newMessage.prompt = Prompt{PromptType{STRING}}; // todo: check with msg team about this formatting and semantics
        newMessage.execState = {executionState->localState.execState};

        // ExecutionState should wait for server response
        executionState->shouldWait = true;

        returnMessages.push_back(newMessage);
    } else {
        std::cout << "Error: could not evaluate text_input node\n";
    }

    return std::make_optional<std::vector<jsonMsg>>(returnMessages);
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_input_vote() {
    std::cout << "This is an input_vote node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_input_range() {
    std::cout << "This is an input_range node\n";
    return std::nullopt;
}

std::optional<std::vector<jsonMsg>> RuleNode::execute_scores() {
    std::cout << "This is a scores node\n";
    return std::nullopt;
}