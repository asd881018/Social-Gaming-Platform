#ifndef DOOZERSTICKS_TREEPARSER_H
#define DOOZERSTICKS_TREEPARSER_H

# pragma once

#include <variant>
#include "cpp-tree-sitter.h"
#include <map>
#include <functional>
#include "RuleNode.h"
#include "NodeVisitor.h"
#include "Server.h" // todo: remove
#include "RuleNode.h"
#include "Expression.h"


/* Temp Data Structure */
struct Configuration {
    std::string name;

    // todo: resolve this conflict
    std::pair<int, int> player_range_NEW;
    ts::Extent<int32_t> player_range;

    bool audience;

    // todo: impl -> JSON?
    std::string tmpSetupString;

    // todo: replace with actual implementation.  maintain for now to preserve game logic dependencies until refactor
    struct Setup {
        int32_t rounds_kind_tmp;
        std::string rounds_prompt_tmp;
        ts::Extent<int32_t> rounds_range_tmp;
    };

    Setup setup;
};


// todo: solidify this.  Use as keys into a map of NodeTypes->Functions?
enum NodeType {
    message,
    parallel_for
};


/**
 * @brief The TreeParser class is responsible for parsing a file using Tree-sitter and extracting relevant information from the syntax tree.
 */
class TreeParser {
public:

    /**
     * @brief Constructs a TreeParser object with the specified filename.
     *
     * @param filename The name of the file to be parsed.
     */
     explicit TreeParser(const std::string &filename);


    /**
     * @brief Returns the root node of the tree.
     *
     * @return The root node of the tree.
     */
    ts::Node getRootNode();

    /**
     * @brief Returns a unique pointer to the root of the rule tree.
     *
     * @return std::unique_ptr<RuleNode> A unique pointer to the root of the rule tree.
     */
    std::unique_ptr<RuleNode> getRuleTreeRoot();

    /**
     * @brief Returns the source string of the parsed file.
     *
     * @return The source string of the parsed file.
     */
    std::string getSourceString() const { return sourceString; }

    /**
     * @brief Reads the contents of a JSON file and returns it as a string.
     *
     * @param filename The name of the JSON file to read.
     * @return A string containing the contents of the JSON file.
     */
    std::string readGameJsonFile(const std::string &filename);

    /** ------------------ Parsing methods ------------------ */
    /**
     * @brief Convenience method to extract the byte range of a Node and return the String representation of this value.
     *
     * @param node The ts node to extract byte range value from.
     * @return std::string Representation of stored byte range value.
     */
    std::string parseNodeValueToString(const ts::Node& node);

    /**
     * @brief Parse a 'number_range' from a Node.
     *
     * @param node The ts node with ts_node_type 'number_range'.
     * @return std::pair Representation of stored byte range value.
     */
    std::pair<int, int> parseNumberRange(const ts::Node& node);

    /**
     * @brief Parse a 'boolean' from a Node.
     *
     * @param node The ts node with ts_node_type 'boolean'.
     * @return bool Representation of stored byte range value.
     */
    bool parseBoolean(const ts::Node& node);

    /**
     * @brief Parse a 'quoted_string' from a Node.
     *
     * @param node The ts node with ts_node_type 'quoted_string'.
     * @return std::string Representation of stored byte range value.
     */
    std::string parseQuotedString(const ts::Node& node);

    /**
     * @brief Parses the given node and returns an optional expression value.
     *
     * @param node The node to parse.
     * @return An optional expression value.
     */
    std::optional<sg::ExpressionValue> parseExpression(const ts::Node& node);

    /**
     * @brief Parse all configuration values from the ts syntax tree.
     *
     * @return Configuration The parsed configuration.
     */
    Configuration parseConfiguration();

    /**
     * @brief Parses the rules node of the tree.
     *
     * @param root The root node of the tree.
     */
    void parseRulesNode(const ts::Node& root);

    /**
     * @brief Parses the body node of the tree with node visitor.
     *
     * @param node The body node to be parsed.
     */
    void parseBodyNode(const ts::Node& node);


    /** ------------------ Debugging Printing Functions ------------------ */
    /**
     * @brief Testing method to print the values and parameters of a given node.
     *
     * @param node The ts node to print info of.
     */
    static void printNodeInfo(const ts::Node& node);


    /**
     * @brief Testing method to print the values and parameters of a given node, as well as the values stored in the associated byte range of the .game file.
     *
     * @param node The ts node to print info of.
     */
    void printNodeInfoWithValue(const ts::Node& node);


    /**
     * @brief Visualizes the tree structure of a parsed Tree-sitter syntax tree, printing all children from a given node.
     *
     * @param root The initial ts node from which to begin visualization.
     */
    void printTree(const ts::Node& root);


    /** ------------------ RuleNodeVisitor ------------------ */
    /**
     * @brief Visitor class for traversing rule nodes in the tree.
     */
    class RuleNodeVisitor : public NodeVisitor {
        void visitRule(const ts::Node& node) override;
        void visitBody(const ts::Node& node) override;
        void visitMessage(const ts::Node& node) override;
        void visitParallelFor(const ts::Node& node) override;
//        void visitFor(const ts::Node& node) override;
//        void visitLoop(const ts::Node& node) override;
//        void visitInParallel(const ts::Node& node) override;
//        void visitMatch(const ts::Node& node) override;
//        void visitExtend(const ts::Node& node) override;
//        void visitReverse(const ts::Node& node) override;
//        void visitShuffle(const ts::Node& node) override;
//        void visitSort(const ts::Node& node) override;
//        void visitDeal(const ts::Node& node) override;
//        void visitDiscard(const ts::Node& node) override;
//        void visitAssignment(const ts::Node& node) override;
//        void visitTimer(const ts::Node& node) override;
//        void visitInputChoice(const ts::Node& node) override;
//        void visitInputText(const ts::Node& node) override;
//        void visitInputVote(const ts::Node& node) override;
//        void visitInputRange(const ts::Node& node) override;
//        void visitScores(const ts::Node& node) override;
    public:
        RuleNodeVisitor(TreeParser& tp) : treeParser(tp) {}

    private:
        TreeParser& treeParser;
    };


    // todo: temp structure, make a part of DataBucket and remove
    std::vector<networking::Connection> Players {
        networking::Connection{1},
        networking::Connection{2},
        networking::Connection{3},
        networking::Connection{4}
    };

    typedef void (*ParseFunction)(const ts::Node&);
    std::unordered_map<NodeType, ParseFunction> ParseMap;

private:
    ts::Language language;
    ts::Parser parser;
    std::string sourceString;
    ts::Tree tree;
    NodeVisitor* visitor;
    std::unique_ptr<RuleNode> ruleTreeRoot;
};

#endif //DOOZERSTICKS_TREEPARSER_H
