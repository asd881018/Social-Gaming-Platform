add_executable(gameServer
  gameServer.cpp
)

target_include_directories(gameServer
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)


target_link_libraries(gameServer
  networking
  gameRoom
  message
  game
  pthread
  messageProcessor
  roomCodeGenerator
)

install(TARGETS gameServer
RUNTIME DESTINATION bin
)