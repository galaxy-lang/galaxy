# Check the minimium CMAKE version <= 3.22

cmake_minimum_required(VERSION 3.22)

# Define the project name

project(parserTest)

# Set the dir sources

set(LEXER_DIR "${CMAKE_SOURCE_DIR}/lexer")
set(PARSER_DIR "{CMAKE_SOURCE_DIR}/parser")
set(NODE_DEFINITIONS_DIR "${CMAKE_SOURCE_DIR}/node_definitions")

# Groups the directories 

file(GLOB LEXER_SOURCES "${LEXER_DIR}/*.c")
file(GLOB PARSER_SOURCES "${PARSER_DIR}/*.c")
file(GLOB NODE_DEFINITIONS_SOURCES "${NODE_DEFINITIONS_DIR}/*.c")

# Set the sources variables

set(SOURCES ${LEXER_SOURCES} ${PARSER_SOURCES} ${NODE_DEFINITIONS_SOURCES})

# Creates a executable called "galaxy" that references the ${SOURCES}

add_executable(galaxy ${SOURCES})

# Create and link a target directories 

target_include_directories(galaxy PRIVATE ${LEXER_DIR} ${PARSER_DIR} ${NODE_DEFINITIONS})
