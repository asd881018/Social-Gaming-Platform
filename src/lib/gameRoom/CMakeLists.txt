add_library(gameRoom
        Setup.cpp
        RunningGame.cpp
        GameRoom.cpp
        RoomDeque.cpp
        GameParams.cpp
        include/GameParams.h
)

target_include_directories(gameRoom
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

install(TARGETS gameRoom
        ARCHIVE DESTINATION lib
        PRIVATE_HEADER DESTINATION include
        PUBLIC_HEADER DESTINATION include
)

target_link_libraries(gameRoom
        networking
        actionNode
        game
        dataBucket
        gameTree
        message
)
