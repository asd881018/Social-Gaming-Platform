// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "GameRunner.h"

// class MockExecutionState : public ExecutionState {
// public:
//     MockExecutionState() : ExecutionState(nullptr, ScopedState{}, nullptr) {}
// };

// // Mock RuleNode for testing
// class MockRuleNode : public RuleNode {
// public:
//     MockRuleNode(const std::string& type, const std::vector<sg::ExpressionValue>& arguments)
//         : RuleNode(type, arguments) {}

//     MOCK_METHOD(std::optional<std::vector<jsonMsg>>, execute, (ExecutionState* exState, GameParams& params), ());
// };

// class GameRunnerTest : public ::testing::Test {
// protected:
//     void SetUp() override {
//     }
//     void TearDown() override {
//     }
// };

// TEST(GameRunnerTest, Construction) {
//     GameParams params;
//     Arguments arg{sg::ExpressionValue{0}};
//     std::unique_ptr<RuleNode> rootNode = std::make_unique<RuleNode>("root", arg);
//     GameRunner gameRunner(std::move(rootNode), params);

//     ASSERT_TRUE(gameRunner.getRoot() != nullptr);
// }

// TEST(GameRunnerTest, Execution) {
//     GameParams params;
//     Arguments arg{sg::ExpressionValue{0}};
//     std::unique_ptr<RuleNode> rootNode = std::make_unique<RuleNode>("root", arg);
//     GameRunner gameRunner(std::move(rootNode), params);

//     jsonMsg inputMessage;
//     inputMessage.execState.id = 1;
//     inputMessage.target.id = "target_id";
//     inputMessage.messageContent = "test_message";

//     auto outputMessages = gameRunner.runGameUntilBlocked(inputMessage);

//     ASSERT_EQ(outputMessages.size(), 1);
//     ASSERT_EQ(outputMessages[0].messageContent, "test_message");
// }

// TEST(GameRunnerTest, IdentifierLookup) {
//     GameParams params;
//     Arguments arg{sg::ExpressionValue{0}};
//     std::unique_ptr<RuleNode> rootNode = std::make_unique<RuleNode>("root", arg);
//     GameRunner gameRunner(std::move(rootNode), params);

//     // Set up a message
//     jsonMsg inputMessage;
//     inputMessage.execState.id = 1;
//     inputMessage.target.id = "target_id";
//     inputMessage.messageContent = "test_message";

//     // Process the message
//     gameRunner.processMessage(inputMessage);

// }
