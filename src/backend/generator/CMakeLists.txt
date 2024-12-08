set(CMAKE_BUILD_TYPE Debug)

# Include global headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather generator sources
file(GLOB_RECURSE GENERATOR_SOURCES "*.cpp")
file(GLOB FRONTEND_SOURCES "${PROJECT_SOURCE_DIR}/src/frontend/**/*.c")
list(FILTER GENERATOR_SOURCES EXCLUDE REGEX ".*generator\\.test\\.cpp$")

# Locate LLVM using llvm-config
find_package(LLVM REQUIRED CONFIG)
execute_process(
    COMMAND llvm-config --cxxflags --ldflags --libs core native
    OUTPUT_VARIABLE LLVM_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Remove leading/trailing spaces and newlines from LLVM_FLAGS
string(REPLACE "\n" "" LLVM_FLAGS "${LLVM_FLAGS}")
string(REPLACE " " ";" LLVM_FLAGS "${LLVM_FLAGS}")

# Include LLVM directories
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Create generator library
add_library(generator ${GENERATOR_SOURCES} ${FRONTEND_SOURCES})
target_link_libraries(generator PUBLIC lexer parser node_definitions ${LLVM_LIBRARIES})

# Create a test executable if test file exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generator.test.cpp")
    add_executable(generator_test generator.test.cpp)
    target_compile_options(generator_test PRIVATE ${LLVM_FLAGS})
    target_link_libraries(generator_test PRIVATE LLVMCore LLVMExecutionEngine LLVMBitWriter LLVMSupport)
    target_link_libraries(generator_test PRIVATE generator lexer parser node_definitions ${LLVM_FLAGS})
    target_include_directories(generator_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif()
