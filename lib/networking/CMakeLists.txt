
add_library(networking
        Server.cpp
        Client.cpp
)

find_package(Threads REQUIRED)

target_include_directories(networking
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        ${CMAKE_SOURCE_DIR}/include/network
        PRIVATE
)

if (NETWORKING_EMSCRIPTEN_BUILD)
    target_link_libraries(networking
            PRIVATE
            websocket.js
    )
else ()
    find_package(Boost 1.72 COMPONENTS system REQUIRED)
    target_link_libraries(networking
            PRIVATE
            ${Boost_LIBRARIES}
            Threads::Threads
    )
    target_include_directories(networking
            PRIVATE
            ${Boost_INCLUDE_DIR}
    )
endif ()

set_target_properties(networking
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
)

install(TARGETS networking
        ARCHIVE DESTINATION lib
)

