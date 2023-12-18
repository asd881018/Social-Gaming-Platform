
add_executable(chatserver
        chatserver.cpp
)

set_target_properties(chatserver
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(chatserver
        networking
        ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(chatserver
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/network
)

install(TARGETS chatserver
        RUNTIME DESTINATION bin
)

