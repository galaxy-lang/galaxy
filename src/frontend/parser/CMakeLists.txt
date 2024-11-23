file(GLOB_RECURSE PARSER_SOURCES "*.c")
list(FILTER PARSER_SOURCES EXCLUDE REGEX ".*parser\\.test\\.c$")

add_library(parser ${PARSER_SOURCES})

target_include_directories(parser PUBLIC ../../../include)

target_link_libraries(parser PUBLIC lexer)

add_executable(parser_test parser.test.c)
target_link_libraries(parser_test PRIVATE parser lexer)

# Certifique-se de que o CMake saiba onde encontrar a biblioteca lexer
target_link_directories(parser_test PRIVATE ../lexer)
