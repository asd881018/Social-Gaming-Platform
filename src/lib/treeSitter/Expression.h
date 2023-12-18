#pragma once
#include <variant>
#include <string>
#include <vector>
#include <map>
#include "cpp-tree-sitter.h"
#include <functional>

enum class ExpressionSymbol {
    Identifier = 85,
    MsgParent = 118,
    Expression = 120,
    PlayerSet = 130,
    // TODO: ... other symbols ...
};

struct Identifier {
    std::string identifier;

    bool operator==(const Identifier &other) const {
        return (identifier == other.identifier);
    }
};

template<>
struct std::hash<Identifier> {
    std::size_t operator()(const Identifier& id) const {
        return (std::hash<std::string>()(id.identifier));
    }
};


namespace sg {
    class ExpressionValue;

    // todo: unordered_map instead? need a map at all?
    using ExpressionValueVariant = std::variant<bool, int, std::string, Identifier, std::vector<ExpressionValue>>;
//    using ExpressionValueVariant = std::variant<bool, int, std::string, Identifier, std::vector<ExpressionValue>, std::unordered_map<Identifier, ExpressionValue>>;

    class ExpressionValue {
    public: // todo: better access

        ExpressionValueVariant expressionValue;

    };

    // todo: convert GameRunner to use these expressions
    struct Expression {
        ExpressionValue value; // can use this field to pass raw ExpressionValues to test interpreter

        ts::Node root; // todo: lifetimes mismatch? group: how to store these with respect to TreeParser?
    };
}


class ValueMap {
public:
    std::unordered_map<Identifier, sg::ExpressionValue> map;
};