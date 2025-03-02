set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)

project(Galaxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Generate libs/std.s and libs/std.o from libs/std.ll
add_custom_command(
  OUTPUT ${PROJECT_SOURCE_DIR}/libs/std.s ${PROJECT_SOURCE_DIR}/libs/std.o  # Generated files
  COMMAND llc ${PROJECT_SOURCE_DIR}/libs/std.ll -o ${PROJECT_SOURCE_DIR}/libs/std.s
  COMMAND clang -c ${PROJECT_SOURCE_DIR}/libs/std.s -o ${PROJECT_SOURCE_DIR}/libs/std.o
  DEPENDS ${PROJECT_SOURCE_DIR}/libs/std.ll  # Input dependency
  COMMENT "Generating libs/std.s and libs/std.o from libs/std.ll"
)

# Mark libs/std.o as an object file
set_source_files_properties(${PROJECT_SOURCE_DIR}/libs/std.o PROPERTIES OBJECT TRUE)

# Create a static library from the generated object file
add_library(llvm_libs_ OBJECT ${PROJECT_SOURCE_DIR}/libs/std.o)

# Create a custom target to ensure the files are generated
add_custom_target(generate__std_files ALL DEPENDS ${PROJECT_SOURCE_DIR}/libs/std.o)

# Add subdirectories for lexer, parser, and node_definitions
add_subdirectory(src/args)
add_subdirectory(src/frontend/lexer)
add_subdirectory(src/frontend/parser)
add_subdirectory(src/frontend/node_definitions)
add_subdirectory(src/backend/generator)

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

# Create a test executable if the test file exists
if(EXISTS "src/main.cpp")
add_executable(galaxy src/main.cpp $<TARGET_OBJECTS:llvm_libs_>)
  target_include_directories(galaxy PRIVATE ${PROJECT_SOURCE_DIR}/include ${LLVM_INCLUDE_DIRS})
  target_compile_options(galaxy PRIVATE ${CMAKE_CXX_FLAGS})
  target_link_libraries(galaxy PRIVATE arg_parse lexer parser node_definitions generator ${LLVM_LIBS} ${LLVM_SYS_LIBS})

  install(TARGETS galaxy RUNTIME DESTINATION bin)
endif()
