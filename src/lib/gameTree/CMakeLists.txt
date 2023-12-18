add_library(gameTree
        NodeActions.cpp
        GameRunner.cpp
        RuleNode.cpp
        ExecutionState.cpp
)

target_include_directories(gameTree
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS gameTree
        ARCHIVE DESTINATION lib
        PRIVATE_HEADER DESTINATION include
        PUBLIC_HEADER DESTINATION include
)

target_link_libraries(gameTree
        dataBucket
        networking
        gameLogic
        message
)