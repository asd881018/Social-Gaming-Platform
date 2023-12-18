# pragma once
//#include <nlohmann/json.hpp>
#include "TreeParser.h"
#include "NodeVisitor.h"


class TreeSitter {
public:
    TreeParser parser;
    TreeSitter(const std::string& filename);
};

/**
 * Generates the root of the rule tree based on the given filename.
 *
 * @param filename The name of the file.
 * @return The root node of the rule tree.
 */
RuleNode* generateRuleTreeRoot(const std::string &filename);
