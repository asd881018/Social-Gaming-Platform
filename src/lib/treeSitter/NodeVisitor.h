//
// Created by matt on 2023-11-03.
//

#ifndef DOOZERSTICKS_NODEVISITOR_H
#define DOOZERSTICKS_NODEVISITOR_H
#include "cpp-tree-sitter.h"
#include "TreeParser.h"

// Define a NodeVisitor interface with visit methods for each node type.
class NodeVisitor {
public:
    virtual void visitRule(const ts::Node& node) = 0;
    virtual void visitBody(const ts::Node& node) = 0;
    virtual void visitMessage(const ts::Node& node) = 0;
    virtual void visitParallelFor(const ts::Node& node) = 0;
//    virtual void visitFor(const ts::Node& node) = 0;
//    virtual void visitLoop(const ts::Node& node) = 0;
//    virtual void visitInParallel(const ts::Node& node) = 0;
//    virtual void visitMatch(const ts::Node& node) = 0;
//    virtual void visitExtend(const ts::Node& node) = 0;
//    virtual void visitReverse(const ts::Node& node) = 0;
//    virtual void visitShuffle(const ts::Node& node) = 0;
//    virtual void visitSort(const ts::Node& node) = 0;
//    virtual void visitDeal(const ts::Node& node) = 0;
//    virtual void visitDiscard(const ts::Node& node) = 0;
//    virtual void visitAssignment(const ts::Node& node) = 0;
//    virtual void visitTimer(const ts::Node& node) = 0;
//    virtual void visitInputChoice(const ts::Node& node) = 0;
//    virtual void visitInputText(const ts::Node& node) = 0;
//    virtual void visitInputVote(const ts::Node& node) = 0;
//    virtual void visitInputRange(const ts::Node& node) = 0;
//    virtual void visitScores(const ts::Node& node) = 0;

};
#endif //DOOZERSTICKS_NODEVISITOR_H
