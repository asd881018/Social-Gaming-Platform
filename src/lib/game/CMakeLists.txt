add_library(game
        GameInfo.cpp
        GameScanner.cpp)

target_include_directories(game
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

