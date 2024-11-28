add_library(args_parser.c)

target_include_directories(args_parser.c PUBLIC ../../include)

target_link_libraries(args_parser.c PRIVATE args)

target_sources(args_parser.c PRIVATE args_parser.c)