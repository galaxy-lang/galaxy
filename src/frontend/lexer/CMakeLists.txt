# Create the lexer library from lexer.c and lexer_error.c
add_library(lexer
    lexer.c
    lexer_error.c
    ../getTokenTypeName.c
    ../freeTokens.c
)

# Include the directory containing header files
#target_include_directories(lexer PUBLIC ../../include)
include_directories(${PROJECT_SOURCE_DIR}/include)

# If lexer.test.c exists, create a separate test executable for lexer
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lexer.test.c")
    add_executable(lexer_test lexer.test.c)
    target_link_libraries(lexer_test PRIVATE lexer)
    #target_include_directories(lexer_test PRIVATE ../../include)
    include_directories(${PROJECT_SOURCE_DIR}/include)
endif()
