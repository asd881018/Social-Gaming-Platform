#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "RuleNode.h"
#include "ExecutionState.h"

class MockExecutionState : public ExecutionState {
public:
    MockExecutionState() : ExecutionState(nullptr, ScopedState{}, nullptr) {}
};

// Mock GameParams for testing
class MockGameParams : public GameParams {
};

// Mock jsonMsg for testing
struct MockJsonMsg {
};

// Mock RuleNode for testing
class MockRuleNode : public RuleNode {
public:
    MockRuleNode(const std::string& type, const std::vector<sg::ExpressionValue>& arguments)
        : RuleNode(type, arguments) {}

    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_message, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_parallel_for, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_for, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_loop, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_in_parallel, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_match, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_extend, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_reverse, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_sort, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_deal, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_discard, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_assignment, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_timer, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_input_choice, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_input_text, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_input_vote, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_input_range, (), ());
    MOCK_METHOD(std::optional<std::vector<MockJsonMsg>>, execute_scores, (), ());
};

class RuleNodeTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

TEST_F(RuleNodeTest, TestAddChild) {
    
    Arguments arg{sg::ExpressionValue{0}};

    std::unique_ptr<RuleNode> rootNode = std::make_unique<RuleNode>("root", arg);

    Arguments messageArg1{sg::ExpressionValue{"all"}, sg::ExpressionValue{"message"}};
    Arguments messageArg2{sg::ExpressionValue{"all"}, sg::ExpressionValue{"message"}};

    rootNode->addChild("message", messageArg1);
    rootNode->addChild("message", messageArg2);

    EXPECT_EQ(rootNode->getChildren().size(), 2);
    EXPECT_EQ(rootNode->getChild(0)->getNodeType(), "message");
    EXPECT_EQ(rootNode->getChild(1)->getNodeType(), "message");

    EXPECT_EQ(rootNode->getChild(0)->getParent(), rootNode.get());
    EXPECT_EQ(rootNode->getChild(1)->getParent(), rootNode.get());
}

TEST_F(RuleNodeTest, TestAddChild2) {
    
    Arguments arg{sg::ExpressionValue{0}};

    std::unique_ptr<RuleNode> rootNode = std::make_unique<RuleNode>("root", arg);

    Arguments messageArg1{sg::ExpressionValue{"all"}, sg::ExpressionValue{"message"}};
    rootNode->addChild("message", messageArg1);

    Arguments messageArg2{sg::ExpressionValue{"all"}, sg::ExpressionValue{"message"}};
    rootNode->getChild(0)->addChild("message", messageArg2);

    EXPECT_EQ(rootNode->getChildren().size(), 1);
    EXPECT_EQ(rootNode->getChild(0)->getNodeType(), "message");
    EXPECT_EQ(rootNode->getChild(0)->getParent(), rootNode.get());


    EXPECT_EQ(rootNode->getChild(0)->getChildren().size(), 1);
    EXPECT_EQ(rootNode->getChild(0)->getChild(0)->getNodeType(), "message");
}


TEST_F(RuleNodeTest, GetNodeArgumentsTest) {
    // Create a RuleNode with some arguments
    Arguments arguments{sg::ExpressionValue{1}, sg::ExpressionValue{2}};
    RuleNode ruleNode("type", arguments);

    auto nodeArguments = ruleNode.getNodeArguments();
    EXPECT_EQ(nodeArguments.size(), 2);
}

TEST_F(RuleNodeTest, GetChildTest) {
    // Create a parent RuleNode with some children
    Arguments parentArguments{sg::ExpressionValue{0}};
    std::unique_ptr<RuleNode> parent = std::make_unique<RuleNode>("parent", parentArguments);

    Arguments childArguments{sg::ExpressionValue{1}};
    parent->addChild("child", childArguments);

    auto& child = parent->getChild(0);
    EXPECT_EQ(child->getNodeType(), "child");
}

TEST_F(RuleNodeTest, GetChildrenTest) {
    // Create a parent RuleNode with some children
    Arguments parentArguments{sg::ExpressionValue{0}};
    std::unique_ptr<RuleNode> parent = std::make_unique<RuleNode>("parent", parentArguments);

    Arguments child1Arguments{sg::ExpressionValue{1}};
    Arguments child2Arguments{sg::ExpressionValue{2}};
    parent->addChild("child1", child1Arguments);
    parent->addChild("child2", child2Arguments);

    auto& children = parent->getChildren();
    EXPECT_EQ(children.size(), 2);
    EXPECT_EQ(children[0]->getNodeType(), "child1");
    EXPECT_EQ(children[1]->getNodeType(), "child2");
}

//TODO: tests for execute methods
// TEST_F(RuleNodeTest, ExecuteMessageTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);

//     // EXPECT_CALL(ruleNode, execute_message()).Times(1);

//     // // Call the execute_message method
//     // auto result = ruleNode.execute_message();
// }

// TEST_F(RuleNodeTest, ExecuteForTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteLoopTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteInParallelTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteMatchTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteExtendTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteReverseTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteSortTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteDealTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteDiscardTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteAssignmentTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteTimerTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteInputChoiceTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteInputTextTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteInputVoteTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteInputRangeTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }

// TEST_F(RuleNodeTest, ExecuteScoresTest) {
//     MockExecutionState executionState;
//     MockGameParams gameParams;

//     Arguments arguments{sg::ExpressionValue{"all"}, sg::ExpressionValue{"Hello"}};
//     MockRuleNode ruleNode("message", arguments);
// }
