#pragma once

#include <string>

#include "GameRunner.h"

class NodeActions {
    explicit NodeActions(GameRunner& gt);

public:
    
    static void message(std::string msg);
    void parallel_for(int numParallel);
    static void forLoop();
private:
    // todo: need this?
    GameRunner* m_gameTree;
};
