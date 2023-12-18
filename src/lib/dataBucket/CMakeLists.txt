set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")

# Add your library
add_library(dataBucket DataBucket.cpp)

target_link_libraries(dataBucket
        nlohmann_json::nlohmann_json
        gameRoom
)
# Make the JSON library available
# FetchContent_MakeAvailable(json)

# Explicitly set the include directory for the json library
target_include_directories(dataBucket
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        PRIVATE ${json_SOURCE_DIR}/include
)



