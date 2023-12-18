#include <iostream>
#include "TreeParser.h"
#include <cassert>
#include <string>
#include <strings.h>
#include <functional>
#include <fstream>
#include <string_view>
#include <tree_sitter/parser.h>
#include <tree_sitter/api.h>
#include <map>

extern "C"
{
TSLanguage* tree_sitter_json();
TSLanguage* tree_sitter_socialgaming();
}

TreeParser::TreeParser(const std::string& filename)
        : language(tree_sitter_socialgaming()), parser(language), sourceString(readGameJsonFile(filename)),
          tree(parser.parseString(sourceString)), visitor(new RuleNodeVisitor(*this)) {

    // todo: proper error handling?
    if (tree.hasError()) {
        std::cout << "TreeParser constructor failed to parse game file \n";
    }

    // Init the rule tree root with empty arguments
    Arguments dummy_arguments{};
    ruleTreeRoot = std::make_unique<RuleNode>("root", dummy_arguments);
//    ruleTreeRoot = RuleNode("root", dummy_arguments);

    // todo: clean this up, possible to use list initialization?
    //    ParseMap.emplace(NodeType::message, &message);
}

ts::Node TreeParser::getRootNode() {
    return tree.getRootNode();
}

std::unique_ptr<RuleNode> TreeParser::getRuleTreeRoot() {
    return std::move(ruleTreeRoot);
}

std::string TreeParser::readGameJsonFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (content.empty()) {
        assert("Failed to read game file" && false);
    }
    return content;
}

std::string TreeParser::parseNodeValueToString(const ts::Node& node) {
    uint32_t startRange = node.getByteRange().start;
    uint32_t endRange = node.getByteRange().end;

    std::string result;
    result.reserve(endRange - startRange);

    // todo: cleaner impl
    for (uint32_t i = startRange; i < endRange; i++) {
        result.push_back(sourceString[i]);
    }

    return result;
}

// todo: testing, error handling
std::pair<int, int> TreeParser::parseNumberRange(const ts::Node& node) {
    // todo: check if type = number_range
    int first, second;

    auto firstChild = node.getNamedChild(0);
    first = std::stoi(parseNodeValueToString(firstChild));

    auto secondChild = node.getNamedChild(1);
    second = std::stoi(parseNodeValueToString(secondChild));

    return std::make_pair(first, second);
}

// todo: testing, error handling
bool TreeParser::parseBoolean(const ts::Node& node) {
    std::string str = parseNodeValueToString(node);

    // parse bool from "true" or "false" string
    // todo: replace with boost lexical_cast
    // todo: strtoi?
    return !str.empty() && (strcasecmp(str.c_str(), "true") == 0 || atoi(str.c_str()) != 0);
}

std::string TreeParser::parseQuotedString(const ts::Node& node) {
    // todo: check for '{' '}' characters, treat as esc char and perform associated lookup if needed
    // Ex: message all "Round {round}. Choose your weapon!";
    try {
        std::string str = parseNodeValueToString(node);
        return str;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: failed to parse quoted string: " << e.what() << "\n";
        return "";
    }
}

// todo: check for dot operator (.) as next child?
// Ex: weapons.name
std::optional<sg::ExpressionValue> TreeParser::parseExpression(const ts::Node& node) {
    sg::ExpressionValue newExpr;
    ExpressionSymbol exprSymbol = static_cast<ExpressionSymbol>(node.getSymbol());

    switch (exprSymbol) {
        // For the expression types that have a single child, parse the child node
        case ExpressionSymbol::Expression:
        case ExpressionSymbol::PlayerSet: {
            ts::Node child = node.getChild(0);
            newExpr.expressionValue = parseQuotedString(child);
            break;
        }
        // For the expression types that have no children, parse the node itself
        case ExpressionSymbol::Identifier:
            newExpr.expressionValue = parseQuotedString(node);
            break;
        default:
            std::cout << "Error: unsupported Expression type: " << node.getType() << "\n";
            break;
    }
    return newExpr;
}

// TODO: return unique_ptr or update an existing?  talk with team
Configuration TreeParser::parseConfiguration() {
    Configuration config;

    ts::Node configNode = getRootNode().getChildByFieldName("configuration");

    //    parseMessage(configNode);

    // TODO: ensure config node exists before accessing
    for (uint32_t i = 0; i < configNode.getNumChildren(); i++) {
        ts::Node node = configNode.getChild(i);
        std::string_view nodeType = node.getType();

        // verify theory: next sibling of 'named' nodes contains associated values
        // todo: cleaner method of doing this - use symbol type
        if (nodeType == "name:") {
            // note: field name = 'name'
            config.name = parseQuotedString(node.getNextSibling());
        } else if (nodeType == "player range:") {
            // note: field name = 'player_range'
            config.player_range_NEW = parseNumberRange(node.getNextSibling());
        } else if (nodeType == "audience:") {
            // note: field name = 'has_audience'
            config.audience = parseBoolean(node.getNextSibling());
        } else if (nodeType == "setup:") {
            // todo: parse each setup_rule as needed [to json? as class?]
        } else {
            //            std::cout << "NO MATCH, type: " << nodeType << "\n";
        }
    }

    // temporary testing, will make use of these variables later to set up the game
    std::cout << "name: " << config.name << "\n";
    std::cout << "player range: " << config.player_range_NEW.first << "-" << config.player_range_NEW.second << "\n";
    std::cout << "bool: " << config.audience << "\n";

    return config;
}

void TreeParser::parseRulesNode(const ts::Node& root) {
    ts::Node rulesNode = root.getChildByFieldName("rules");
    ts::Node bodyNode = rulesNode.getChildByFieldName("body");
    parseBodyNode(bodyNode);
}

// Example using a map for type to function mapping
static const std::unordered_map<std::string_view, void (NodeVisitor::*)(const ts::Node&)> visitFunctions = {
        {"rule",         &NodeVisitor::visitRule},
        {"body",         &NodeVisitor::visitBody},
        {"message",      &NodeVisitor::visitMessage},
        {"parallel_for", &NodeVisitor::visitParallelFor},
        //                {"for", &NodeVisitor::visitFor},
        //                {"loop", &NodeVisitor::visitLoop},
        //                {"in_parallel", &NodeVisitor::visitInParallel},
        //                {"match", &NodeVisitor::visitMatch},
        //                {"extend", &NodeVisitor::visitExtend},
        //                {"reverse", &NodeVisitor::visitReverse},
        //                {"shuffle", &NodeVisitor::visitShuffle},
        //                {"sort", &NodeVisitor::visitSort},
        //                {"deal", &NodeVisitor::visitDeal},
        //                {"discard", &NodeVisitor::visitDiscard},
        //                {"assignment", &NodeVisitor::visitAssignment},
        //                {"timer", &NodeVisitor::visitTimer},
        //                {"input_choice", &NodeVisitor::visitInputChoice},
        //                {"input_text", &NodeVisitor::visitInputText},
        //                {"input_vote", &NodeVisitor::visitInputVote},
        //                {"input_range", &NodeVisitor::visitInputRange},
        //                {"scores", &NodeVisitor::visitScores}
};

void TreeParser::parseBodyNode(const ts::Node& node) {
    if (!visitor)
        return;

    for (uint32_t i = 0; i < node.getNumChildren(); ++i) {
        const auto& child = node.getChild(i);
        const std::string_view type = child.getType();

        auto it = visitFunctions.find(type);
        if (it != visitFunctions.end()) {
            (visitor->*(it->second))(child);
        }

        if (child.getNumChildren() > 0) {
            parseBodyNode(child);
        }
    }
}



// TODO: Implement the behavior for the relevant node types.
void TreeParser::RuleNodeVisitor::visitRule(const ts::Node& node) {
    std::cout << "This is a rule node\n";
}

void TreeParser::RuleNodeVisitor::visitBody(const ts::Node& node) {
    std::cout << "This is a body node\n";
}

void TreeParser::RuleNodeVisitor::visitMessage(const ts::Node& node) {
    ExpressionSymbol exprSymbol = static_cast<ExpressionSymbol>(node.getSymbol());

    if (exprSymbol == ExpressionSymbol::MsgParent) {
        ts::Node playerSetNode = node.getChild(1);
        ts::Node expressionNode = node.getChild(2);
        std::optional<sg::ExpressionValue> playerSetExpr = treeParser.parseExpression(playerSetNode);
        std::optional<sg::ExpressionValue> exprExpr = treeParser.parseExpression(expressionNode);

        Arguments arguments;
        arguments.push_back(*playerSetExpr);
        arguments.push_back(*exprExpr);

        // Append the RuleNode to the RuleNode tree
        treeParser.ruleTreeRoot->addChild("message", arguments);

        // todo: make something like this work
        // std::cout << "expression:  " << std::visit(PrintVisitor, intFloat) << "\n";
    }
}

void TreeParser::RuleNodeVisitor::visitParallelFor(const ts::Node& node) {
    ts::Node identifierNode = node.getChild(1);
    ts::Node exprExprNode = node.getChild(3);

    std::optional<sg::ExpressionValue> identifierExpr = treeParser.parseExpression(identifierNode);
    std::optional<sg::ExpressionValue> exprExpr = treeParser.parseExpression(exprExprNode);

    Arguments arguments;
    arguments.push_back(*identifierExpr);
    arguments.push_back(*exprExpr);

    // Append the RuleNode to the RuleNode tree
    treeParser.ruleTreeRoot->addChild("parallel_for", arguments);
}

/** ------------------ Debuggin Printing Functions ------------------ */


void TreeParser::printNodeInfo(const ts::Node& node) {
    std::cout << "-----\n";
    std::cout << "node type: " << node.getType() << "\n";
    std::cout << "symbol: " << node.getSymbol() << "\n";
    std::cout << "byte range: " << node.getByteRange().start << " to " << node.getByteRange().end << "\n";
    std::cout << "num children: " << node.getNumChildren() << "\n";
    std::cout << "-----\n";
}

void TreeParser::printNodeInfoWithValue(const ts::Node& node) {
    printNodeInfo(node);
    std::cout << "value: " << parseNodeValueToString(node) << "\n";
    std::cout << "-----\n";
}

void TreeParser::printTree(const ts::Node& root) {
    std::function<void(const ts::Node&, int)> printTreeHelper;
    printTreeHelper = [this, &printTreeHelper](const ts::Node& root, int indentation) {
        for (uint32_t i = 0; i < root.getNumChildren(); ++i) {
            ts::Node node = root.getChild(i);

            for (int j = 0; j < indentation; ++j) {
                std::cout << "    ";
            }

            std::cout << node.getType() << " [" << node.getSymbol() << "]";

            if (node.getNumChildren() == 0) {
                std::cout << " -> value: " << TreeParser::parseNodeValueToString(node) << "\n";
            } else {
                std::cout << " (" << node.getNumChildren() << " children)\n";
            }

            if (node.getNumChildren() > 0) {
                printTreeHelper(node, indentation + 1);
            }
        }
    };

    int initialIndentation = 0;
    printTreeHelper(root, initialIndentation);
}




