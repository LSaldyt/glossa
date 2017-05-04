cmake_minimum_required (VERSION 2.8.7)
cmake_policy(VERSION 2.8.7)

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

add_library(glossalib SHARED "${LIB_SOURCES}")
add_executable(glossa "${PROG_SOURCES}")
target_link_libraries(glossa glossalib)
add_executable(glossatest "${TEST_SOURCES}")
target_link_libraries(glossatest glossalib)
