# Add all .c files in the parser directory, excluding parser.test.c
file(GLOB_RECURSE PARSER_SOURCES "*.c")
list(FILTER PARSER_SOURCES EXCLUDE REGEX ".*parser\\.test\\.c$")

# Create the parser library from the gathered source files
add_library(parser ${PARSER_SOURCES})
set(CMAKE_BUILD_TYPE Debug)


# Include the directory containing header files
#target_include_directories(parser PUBLIC ../../include)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link the parser library with lexer and node_definitions libraries
target_link_libraries(parser PUBLIC lexer node_definitions)

# If parser.test.c exists, create a separate test executable for parser
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/parser.test.c")
    add_executable(parser_test parser.test.c)
    target_link_libraries(parser_test PRIVATE parser lexer node_definitions)
    target_include_directories(parser_test PRIVATE ../../include)
endif()
