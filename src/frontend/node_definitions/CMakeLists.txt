# Create the node_definitions library from ast.c
add_library(node_definitions ast.c)

# Include the directory containing header files
#target_include_directories(node_definitions PUBLIC ../../include)
include_directories(${PROJECT_SOURCE_DIR}/include)
