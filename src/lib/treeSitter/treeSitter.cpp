#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "treeSitter.h"


/** Define the path to the game configuration file
 *  To run the demo, make sure you are at the bin directory of the build directory
 * */
#define ROCK_PAPER_SCISSORS_GAME "./resource/games/rock-paper-scissors.game"
#define HELLO_TREE_SITTER_GAME "./resource/games/hello-tree-sitter.game"
#define SIMPLE_MESSAGE_GAME "./resource/games/simple-message.game"

RuleNode* generateRuleTreeRoot(const std::string& filename) {

    TreeParser tp{filename};
    ts::Node root = tp.getRootNode();
    tp.parseRulesNode(root);
//    RuleNode* ruleTreeRoot = tp.getRuleTreeRoot();
    std::unique_ptr<RuleNode> ruleTreeRoot = tp.getRuleTreeRoot();
    return ruleTreeRoot.get();

//    Configuration config = tp.parseConfiguration();

/* Debugging printing*/
//    tp.printTree(root);
}