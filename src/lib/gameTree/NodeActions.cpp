#include <iostream>

#include "NodeActions.h"

// todo: copying?  look more at this impl as ownership becomes clearer
NodeActions::NodeActions(GameRunner& gt) {
    m_gameTree = &gt;
}

//void NodeActions::message(std::string msg) {
//    std::cout << msg << "\n";
//}
//
//void NodeActions::forLoop() {
//    std::cout << "This is a for node\n";
//}

//void NodeActions::parallel_for(int numParallel) {

//    m_gameTree.createExecutionState()
//}


