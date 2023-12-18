#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TreeParser.h"

#define ROCK_PAPER_SCISSORS_GAME "./resource/games/rock-paper-scissors.game"
#define HELLO_TREE_SITTER_GAME "./resource/games/hello-tree-sitter.game"

class TreeParserTest : public ::testing::Test {
public:

    TreeParser tp{HELLO_TREE_SITTER_GAME};

    ts::Node root = tp.getRootNode();
    // tp.parseRulesNode(root);
};

TEST_F(TreeParserTest, ReadGameJsonFileTest) {
    std::string filename = HELLO_TREE_SITTER_GAME;
    std::string expectedContent = "// Demo of simpler game for testing basic rule parsing semantics\n"
                                  "// V.1.1\n"
                                  "configuration {\n"
                                  "  name: \"Hello, tree-sitter\"\n"
                                  "  player range: (2, 4)\n"
                                  "  audience: false\n"
                                  "  setup: {\n"
                                  "    rounds {\n"
                                  "      kind: integer\n"
                                  "      prompt: \"The number of rounds to play\"\n"
                                  "      range: (1, 20)\n"
                                  "    }\n"
                                  "  }\n"
                                  "}\n"
                                  "\n"
                                  "// Global constants can be expressed in a JSON-like format.\n"
                                  "constants {\n"
                                  "  weapons: [\n"
                                  "    { name: \"Rock\",     beats: \"Scissors\" },\n"
                                  "    { name: \"Paper\",    beats: \"Rock\"     },\n"
                                  "    { name: \"Scissors\", beats: \"Paper\"    },\n"
                                  "  ]\n"
                                  "}\n"
                                  "\n"
                                  "// The same is true of the initial values for global variables.\n"
                                  "variables {\n"
                                  "  winners: []\n"
                                  "}\n"
                                  "\n"
                                  "// The per player state exists independently for each player, so\n"
                                  "// initializing `wins` below to 0 creates a counter for each player and\n"
                                  "// initializes it to 0.\n"
                                  "per-player {\n"
                                  "  wins: 0\n"
                                  "}\n"
                                  "\n"
                                  "// If there were some form of audience participation, the state for each\n"
                                  "// audience member could also be tracked separately. What sorts of\n"
                                  "// considerations might that imply for implementing it?\n"
                                  "per-audience {}\n"
                                  "\n"
                                  "// Finally, the rules of the game execute as previously described after the\n"
                                  "// game has been started.\n"
                                  "rules {\n"
                                  "    message all \"Choose your weapon!\"; // node 1\n"
                                  "\n"
                                  "    parallel for player in players { // node 2\n"
                                  "          input choice to player { //node that's a child of node 2\n"
                                  "            prompt: \"{player.name}, choose your weapon!\"\n"
                                  "            choices: weapons.name\n"
                                  "            target: player.name\n"
                                  "            timeout: 10\n"
                                  "          }\n"
                                  "        }\n"
                                  "\n"
                                  "    message all \"Winners: {winners.elements.name}\"; //node 3\n"
                                  "}";

    std::string content = tp.readGameJsonFile(filename);

    EXPECT_EQ(content, expectedContent);
}

TEST_F(TreeParserTest, TreeParserConstructorTest) {
    std::string filename = HELLO_TREE_SITTER_GAME;

    TreeParser tp(filename);

    // Verify that the source string is read correctly
    EXPECT_EQ(tp.getSourceString(), tp.readGameJsonFile(filename));

//    // Verify that the tree is parsed correctly
//    EXPECT_FALSE(tp.tree.hasError());

}

TEST_F(TreeParserTest, ParseNodeValueToStringTest) {


    // Call the parseNodeValueToString function
    std::string result = tp.parseNodeValueToString(root.getNamedChild(0));

    // Verify the result
    EXPECT_EQ(result, "// Demo of simpler game for testing basic rule parsing semantics");
}

TEST_F(TreeParserTest, ParseConfigurationTest) {
    Configuration expectedConfig;
    expectedConfig.name = "\"Hello, tree-sitter\"";
    expectedConfig.player_range_NEW = std::make_pair(2, 4);
    expectedConfig.audience = false;

    Configuration actualConfig = tp.parseConfiguration();

    EXPECT_EQ(actualConfig.name, expectedConfig.name);
    EXPECT_EQ(actualConfig.player_range_NEW, expectedConfig.player_range_NEW);
    EXPECT_EQ(actualConfig.audience, expectedConfig.audience);
}

TEST_F(TreeParserTest, ParseQuotedStringTest) {
    std::string expectedString = "// Demo of simpler game for testing basic rule parsing semantics";
    ts::Node node = root.getNamedChild(0); // Assuming this node represents the quoted string
    std::string actualString = tp.parseQuotedString(node);

    EXPECT_EQ(actualString, expectedString);
}

// TEST_F(TreeParserTest, ParseExpressionTest) {
//     // Create a sample node for testing
//     tp.parseRulesNode(root);


//     // Call the parseExpression function
//     std::optional<sg::ExpressionValue> result = tp.parseExpression(node);

//     // Verify the result
//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result.value().getType(), sg::ExpressionType::Integer);
//     EXPECT_EQ(result.value().getIntegerValue(), 42);
// }

// TEST_F(TreeParserTest, ParseExpressionInvalidTest) {
//     // Create a sample node for testing
//     ts::Node node = root.getNamedChild(2); // Assuming this node represents an invalid expression

//     // Call the parseExpression function
//     std::optional<sg::ExpressionValue> result = tp.parseExpression(node);

//     // Verify the result
//     EXPECT_FALSE(result.has_value());
// }



