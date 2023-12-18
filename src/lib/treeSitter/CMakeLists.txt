

# Downloads a tree-sitter grammar from github and makes it available as a
# cmake library target.
add_grammar_from_repo(tree-sitter-json                 # Defines the library name for a grammar
        https://github.com/tree-sitter/tree-sitter-json.git  # Repository URL of a tree-sitter grammar
        0.19.0                                               # Version tag for the grammar
)

add_grammar_from_repo(tree-sitter-socialgaming                 # Defines the library name for a grammar
       https://github.com/nsumner/tree-sitter-socialgaming.git  # Repository URL of a tree-sitter-socialgaming  grammar
       0.0.1                                               # Version tag for the grammar
)

add_library(treeSitter treeSitter.cpp
        TreeParser.h
        TreeParser.cpp
        NodeVisitor.h
        Expression.h
)


target_include_directories(treeSitter
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(treeSitter PROPERTIES PUBLIC_HEADER "include/treeSitter.h")

# copy all games in source to build directory
file(COPY ${CMAKE_SOURCE_DIR}/resource/games
        DESTINATION ${CMAKE_BINARY_DIR}/resource)

# Use the library in a demo program.
target_link_libraries(treeSitter
        tree-sitter-json
        tree-sitter-socialgaming
        cpp-tree-sitter
        nlohmann_json::nlohmann_json
        message
        gameTree
)
