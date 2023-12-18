add_library(clientHelper clientHelper.cpp)

target_include_directories(clientHelper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS clientHelper
  ARCHIVE DESTINATION lib
)

target_link_libraries(clientHelper
  networking
  message
  pthread
  gameRoom
  )


