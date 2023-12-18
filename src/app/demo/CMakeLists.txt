add_executable(demo
        demo.cpp
)

# Add CPP Modern JSON
# CPMAddPackage("gh:nlohmann/json@3.10.5")

#include(FetchContent)
#FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
#FetchContent_MakeAvailable(json)


target_include_directories(demo
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/lib
)



target_link_libraries(demo
        networking
        treeSitter
        actionNode
        gameLogic
        gameTree
        nlohmann_json::nlohmann_json
)
