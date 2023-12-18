add_library(message msgParser.cpp msgCreator.cpp
        StringResources.h)

target_include_directories(message
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${json_SOURCE_DIR/include}
)


install(TARGETS message
  ARCHIVE DESTINATION lib
)

target_link_libraries(message networking)
target_link_libraries(message gameRoom)
target_link_libraries(message nlohmann_json)

FetchContent_MakeAvailable(json)


