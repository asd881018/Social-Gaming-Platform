# Add the gtest subdirectory for Google Test and Google Mock
add_subdirectory(gtest)

add_executable(runAllTests
        lib/gameRoom/GameRoom_test.cpp
        lib/gameRoom/Poll_test.cpp
        lib/gameRoom/GameRunner_test.cpp
        lib/dataBucket/DataBucket_test.cpp
        lib/game/GameManager_test.cpp
        lib/game/Game_test.cpp
        lib/TreeSitter/TreeSitter_test.cpp
        lib/TreeParser/TreeParser_test.cpp
        lib/roomCodeGenerator/RoomCodeGenerator_test.cpp
        lib/messageProcessor/MessageProcessor_test.cpp
        lib/clientHelper/clientHelper_test.cpp
        lib/gameTree/RuleNode_test.cpp
        lib/gameTree/GameRunner_test.cpp
)

target_include_directories(runAllTests
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(runAllTests
        PRIVATE
        GTest::gmock GTest::gtest GTest::gtest_main
        networking
        gameRoom
        dataBucket
        message
        game
        treeSitter
        roomCodeGenerator
        messageProcessor
        gameTree
        clientHelper
        #TreeParser
)

add_test(NAME GTestTest COMMAND runAllTests)