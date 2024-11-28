cmake_minimum_required(VERSION 3.10)
project(Galaxy)

# Define the main executable from the main.c file
add_executable(galaxy src/main.c)
target_include_directories(galaxy PRIVATE include)

# Link the lexer library to galaxy
target_link_libraries(galaxy PRIVATE lexer parser arg_parse)

# Add subdirectories for lexer, parser, and node_definitions
add_subdirectory(src/frontend/lexer)
add_subdirectory(src/frontend/parser)
add_subdirectory(src/frontend/node_definitions)
add_subdirectory(src/args)
