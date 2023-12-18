add_library(messageProcessor MessageProcessor.cpp)

target_include_directories(messageProcessor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS messageProcessor
  ARCHIVE DESTINATION lib
)

target_link_libraries(messageProcessor
        networking
        gameRoom
        message
        roomCodeGenerator
)


