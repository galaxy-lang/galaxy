cmake_minimum_required(VERSION 3.10)
project(Galaxy)

# Define the main executable from the main.c file
add_executable(galaxy src/main.c)
target_include_directories(galaxy PRIVATE include)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link the lexer library to galaxy
target_link_libraries(galaxy PRIVATE arg_parse lexer parser node_definitions generator)
set(CMAKE_BUILD_TYPE Debug)

# Add subdirectories for lexer, parser, and node_definitions
add_subdirectory(src/args)
add_subdirectory(src/frontend/lexer)
add_subdirectory(src/frontend/parser)
add_subdirectory(src/frontend/node_definitions)
add_subdirectory(src/backend/generator)
