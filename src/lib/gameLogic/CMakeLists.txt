add_library(gameLogic
        src/GameController.cpp
        src/GameModel.cpp
        src/Player.cpp
        src/GameView.cpp
        src/NodeHelpers.cpp)

target_include_directories(gameLogic
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

set_target_properties(gameLogic PROPERTIES 
    PUBLIC_HEADER "include/gameController.h;include/GameModel.h;include/GameView.h;include/Player.h;include/NodeHelpers.h"
)

INSTALL(TARGETS gameLogic
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)

target_link_libraries(gameLogic actionNode treeSitter)