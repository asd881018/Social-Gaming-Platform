add_library(roomCodeGenerator RoomCodeGenerator.cpp)

target_include_directories(roomCodeGenerator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS roomCodeGenerator
  ARCHIVE DESTINATION lib
)

target_link_libraries(roomCodeGenerator
        message
)
