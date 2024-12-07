set(CMAKE_BUILD_TYPE Debug)

# Include global headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather generator sources
file(GLOB_RECURSE GENERATOR_SOURCES "*.cpp")
file(GLOB FRONTEND_SOURCES "${PROJECT_SOURCE_DIR}/src/frontend/**/*.c")
list(FILTER GENERATOR_SOURCES EXCLUDE REGEX ".*generator\\.test\\.cpp$")

# Add any missing source files explicitly if needed
# For example, if tokenize is implemented in tokenize.c, add it like this:
# list(APPEND FRONTEND_SOURCES "${PROJECT_SOURCE_DIR}/src/frontend/lexer/tokenize.c")

# Create generator library
add_library(generator ${GENERATOR_SOURCES} ${FRONTEND_SOURCES})

# Link with other components
target_link_libraries(generator PUBLIC lexer parser node_definitions)

# Create a test executable if test file exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generator.test.cpp")
    add_executable(generator_test generator.test.cpp)
    target_link_libraries(generator_test PRIVATE generator lexer parser node_definitions)
    target_include_directories(generator_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif()