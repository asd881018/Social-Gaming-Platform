add_library(actionNode
        output/MessageAllNode.cpp
        control/InOrderNode.cpp
        input/InputTextNode.cpp
        control/InParallelNode.cpp
        timing/TimerNode.cpp
        include/Node.h
        include/DummyBucket.h
)

target_include_directories(actionNode
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(actionNode
        PRIVATE
            networking
            gameLogic
)

set_target_properties(actionNode PROPERTIES
        PUBLIC_HEADER "include/NodeImpl.h"
        PUBLIC_HEADER "include/MessageAllNode.h"
        PUBLIC_HEADER "include/InOrderNode.h"
        PUBLIC_HEADER "include/InputTextNode.h"
        PUBLIC_HEADER "include/InParallelNode.h"
        PUBLIC_HEADER "include/TimerNode.h"
        PUBLIC_HEADER "include/Node.h"
        PUBLIC_HEADER "include/DummyBucket.h"
)