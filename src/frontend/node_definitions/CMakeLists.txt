add_library(node_definitions STATIC ast.c)

target_include_directories(node_definitions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
