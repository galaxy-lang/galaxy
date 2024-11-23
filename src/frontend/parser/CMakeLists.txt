#cmake_minimum_required(VERSION 3.22)

#project(parserTest)

#add_executable(galaxy parser.test.c)

#add_library(parser
#    parser.c
#    parser.test.c
#)

#target_include_directories(parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../node_definitions)

#target_link_libraries(parser PRIVATE node_definitons)


project(parserTest)

cmake_minimum_required(VERSION 3.22)

set(AST_DIR "${CMAKE_SOURCE_DIR}/../node_definitions")
set(PARSER_DIR "${CMAKE_SOURCE_DIR}")

file(GLOB AST_SOURCES "${AST_DIR}/*.c")
file(GLOB PARSER_SOURCES "${PARSER_DIR}/*.c")

set(SOURCES ${AST_SOURCES} ${PARSER_SOURCES})

add_executable(galaxy ${SOURCES})

target_include_directories(parserTest PRIVATE ${AST_DIR} ${PARSER_DIR})
