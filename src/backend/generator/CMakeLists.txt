set(CMAKE_BUILD_TYPE Debug)

# Include global headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather generator sources
file(GLOB_RECURSE GENERATOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB FRONTEND_SOURCES "${PROJECT_SOURCE_DIR}/src/frontend/**/*.c")
list(FILTER GENERATOR_SOURCES EXCLUDE REGEX ".*generator\\.test\\.cpp$")

# Locate LLVM using llvm-config
find_package(LLVM REQUIRED CONFIG)

# Retrieve LLVM flags
execute_process(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE CMAKE_CXX_FLAGS)
execute_process(COMMAND llvm-config --libs OUTPUT_VARIABLE LLVM_LIBS)
execute_process(COMMAND llvm-config --system-libs OUTPUT_VARIABLE LLVM_SYS_LIBS)
execute_process(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS)

# Strip trailing spaces and newlines
string(STRIP "${LLVM_LIBS}" LLVM_LIBS)
string(STRIP "${LLVM_SYS_LIBS}" LLVM_SYS_LIBS)
string(STRIP "${LLVM_LDFLAGS}" LLVM_LDFLAGS)
string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

# Include LLVM directories
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Link LLVM libraries
link_libraries(${LLVM_LIBS} ${LLVM_SYS_LIBS} ${LLVM_LDFLAGS})

# Create generator library
add_library(generator ${GENERATOR_SOURCES} ${FRONTEND_SOURCES})
target_include_directories(generator PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(generator PUBLIC lexer parser node_definitions ${LLVM_LIBS} ${LLVM_SYS_LIBS})

# Create a test executable if the test file exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generator.test.cpp")
    add_executable(generator_test ${CMAKE_CURRENT_SOURCE_DIR}/generator.test.cpp)
    target_include_directories(generator_test PRIVATE ${PROJECT_SOURCE_DIR}/include ${LLVM_INCLUDE_DIRS})
    target_compile_options(generator_test PRIVATE ${CMAKE_CXX_FLAGS})
    target_link_libraries(generator_test PRIVATE generator lexer parser node_definitions ${LLVM_LIBS} ${LLVM_SYS_LIBS})
endif()
