add_library(arg_parse parser/arg_parser.c)
target_include_directories(arg_parse PUBLIC ${CMAKE_SOURCE_DIR}/include/args)
