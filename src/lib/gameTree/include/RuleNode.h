#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include "DataBucket.h"
#include "msgCreator.h"
#include "Expression.h"
#include "GameParams.h"

// forward declaration
class ExecutionState;

// todo: temp, replace with actual type (unordered_map?), use vector of Expression eventually
typedef std::vector<sg::ExpressionValue> Arguments;

struct ScopedState
{
    // todo: use actual DataBucket
    //    DataBucket valueMap;
    ValueMap valueMap;
    ExecState execState{};
};

class RuleNode
{
public:
    /**
     * @brief Constructs a RuleNode object with the given type and arguments.
     *
     * @param type The type of the RuleNode.
     * @param arguments The arguments of the RuleNode.
     */
    RuleNode(std::string type, std::vector<sg::ExpressionValue> arguments);

    /**
     * Adds a child node to the RuleNode.
     *
     * @param type The type of the child node.
     * @param arguments The arguments for the child node.
     */
    void addChild(const std::string &type, const Arguments &arguments);

    /**
     * @brief Adds a child node to the current RuleNode.
     *
     * @param newChildNode The new child node to be added.
     */
    void addChild(std::unique_ptr<RuleNode> newChildNode);

    /**
     * Executes the rule node and returns the result.
     *
     * @param exState The execution state of the game.
     * @param params The game parameters.
     * @return An optional vector of jsonMsg representing the result of the execution.
     */
    std::optional<std::vector<jsonMsg>> execute(ExecutionState *exState, GameParams &params);

    [[nodiscard]] uint32_t getNodeId() const;
    [[nodiscard]] RuleNode *getParent();
    [[nodiscard]] std::string getNodeType() const;
    [[nodiscard]] std::vector<sg::ExpressionValue> getNodeArguments() const;
    [[nodiscard]] std::unique_ptr<RuleNode> &getChild(int index);
    [[nodiscard]] std::vector<std::unique_ptr<RuleNode>> &getChildren();

    void print();

private:
    std::vector<std::unique_ptr<RuleNode>> children;

    // todo: store actual types instead of std::string
    std::string nodeType;
    Arguments nodeArguments;

    // store pointers to execute() arguments to allow map of handlers to function
    GameParams *gameParameters;
    //    ScopedState* localState;
    ExecutionState *executionState;

    uint32_t nodeId; // todo: actual implementation, increment?  unique?
    RuleNode *parent = nullptr;

    /**
     * @brief A map that associates string keys with function pointers of type RuleNode::NodeExecuter.
     *
     * This map is used to store the mapping between string keys and the corresponding function pointers
     * for executing different types of rule nodes in the RuleNode class.
     */
    using NodeExecuter = std::function<std::optional<std::vector<jsonMsg>>(RuleNode *)>;
    static std::map<std::string, NodeExecuter> executerMap;

    std::optional<std::vector<jsonMsg>> execute_message();
    std::optional<std::vector<jsonMsg>> execute_parallel_for();
    std::optional<std::vector<jsonMsg>> execute_for();
    std::optional<std::vector<jsonMsg>> execute_loop();
    std::optional<std::vector<jsonMsg>> execute_in_parallel();
    std::optional<std::vector<jsonMsg>> execute_match();
    std::optional<std::vector<jsonMsg>> execute_extend();
    std::optional<std::vector<jsonMsg>> execute_reverse();
    std::optional<std::vector<jsonMsg>> execute_sort();
    std::optional<std::vector<jsonMsg>> execute_deal();
    std::optional<std::vector<jsonMsg>> execute_discard();
    std::optional<std::vector<jsonMsg>> execute_assignment();
    std::optional<std::vector<jsonMsg>> execute_timer();
    std::optional<std::vector<jsonMsg>> execute_input_choice();
    std::optional<std::vector<jsonMsg>> execute_input_text();
    std::optional<std::vector<jsonMsg>> execute_input_vote();
    std::optional<std::vector<jsonMsg>> execute_input_range();
    std::optional<std::vector<jsonMsg>> execute_scores();
};
