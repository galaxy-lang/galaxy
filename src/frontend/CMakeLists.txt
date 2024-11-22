cmake_minimum_required(VERSION 3.22)

project(galaxyTest)

add_subdirectory(parser)
add_subdirectory(lexer)

add_executable(galaxy ../main.c)

target_link_libraries(galaxy parser lexer)

