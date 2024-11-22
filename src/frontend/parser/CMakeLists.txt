add_library(parser
    parser.c
    parser.test.c
)

target_include_directories(parser PUBLIC ../../../include)
