
# Define the main executable from the ../main.c file
add_library(arg_parse arg_parse.c)
target_include_directories(arg_parser PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Add subdirectories for args_parser
add_subdirectory(./parser)