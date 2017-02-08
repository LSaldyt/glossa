cmake_minimum_required (VERSION 2.8.10)
cmake_policy(VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0")

file (GLOB_RECURSE LIB_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp" # Skip Compiler.hpp and Compiler.cpp, which include int main()
    "${CMAKE_SOURCE_DIR}/src/*/*.hpp")
file (GLOB_RECURSE PROG_SOURCES
    "${CMAKE_SOURCE_DIR}/src/compiler.cpp"
    "${CMAKE_SOURCE_DIR}/src/compiler.hpp")
file (GLOB_RECURSE TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/tests/*.cpp" # Skip Compiler.hpp and Compiler.cpp, which include int main()
    "${CMAKE_SOURCE_DIR}/tests/*.hpp")

add_library(progtranlib SHARED "${LIB_SOURCES}")
add_executable(progtran "${PROG_SOURCES}")
target_link_libraries(progtran progtranlib)
add_executable(progtrantest "${TEST_SOURCES}")
target_link_libraries(progtrantest progtranlib)
