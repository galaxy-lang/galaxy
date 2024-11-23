cmake_minimum_required(VERSION 3.10)
project(GalaxyFrontend)

# Define the main executable from the ../main.c file
add_executable(galaxy_main ../main.c)
target_include_directories(galaxy_main PRIVATE ../../include)

# Link the lexer library to galaxy_main
target_link_libraries(galaxy_main PRIVATE lexer parser)

# Add subdirectories for lexer, parser, and node_definitions
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(node_definitions)
