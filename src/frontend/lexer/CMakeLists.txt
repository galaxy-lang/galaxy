add_library(lexer
    lexer.c
    lexer_error.c
)

enable_testing()

add_executable(lexer_tests lexer.test.c)
target_link_libraries(lexer_tests lexer)
add_test(NAME LexerTest COMMAND lexer_tests)

target_include_directories(lexer PUBLIC ../../../include)

