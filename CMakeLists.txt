cmake_minimum_required(VERSION 3.10)
project(GalaxyLang VERSION 1.0 LANGUAGES C)

include_directories(include)

add_subdirectory(src/frontend/lexer)

add_executable(galaxy src/main.c)

target_link_libraries(galaxy lexer)
