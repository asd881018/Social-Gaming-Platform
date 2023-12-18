cmake_minimum_required(VERSION 3.24)
project(DoozerSticks)

set(DBoost_NO_BOOST_CMAKE=ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-rtti")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(NETWORKING_EMSCRIPTEN_BUILD ON)
    set(CMAKE_CXX_FLAGS_DEBUG
            "-O0 -g -fno-omit-frame-pointer"
    )
else ()
    set(NETWORKING_EMSCRIPTEN_BUILD OFF)
    set(CMAKE_CXX_FLAGS_DEBUG
            "-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer"
    )
endif ()

option(ENABLE_FLUTTER_CLIENT "Build the Flutter client" ON)
option(ENABLE_FTXUI_CLIENT "Build the FTXUI client" ON)
option(ENABLE_NCURSES_CLIENT "Build the NCurses client" ON)

add_subdirectory(lib)
add_subdirectory(example)
add_subdirectory(test)
add_subdirectory(src)

