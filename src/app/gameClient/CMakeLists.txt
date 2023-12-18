
add_executable(gameClient
  newClient.cpp
)

set_target_properties(gameClient
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

target_include_directories(gameClient
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/lib/message
        ${CMAKE_SOURCE_DIR}/src/lib/gameRoom
)

target_link_libraries(gameClient
  networking
  message
  pthread
  gameRoom
  clientHelper
)

install(TARGETS gameClient
  RUNTIME DESTINATION bin
)

