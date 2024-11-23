
# Check the minimum version of CMake <= 3.22
cmake_minimum_required(VERSION 3.22)

# Define the project name as galaxyTest

project(galaxyTest)

# Add the subdirectories "parser" and "lexer"

add_subdirectory(parser)
add_subdirectory(lexer)

# Creates a executale called galaxy that references main.c in galaxy/src

add_executable(parser_test ./parser/parser.test.c)

# link the libraries parser / lexer

target_link_libraries(parser lexer)

target_include_directories(lexer PUBLIC ../../../include)
