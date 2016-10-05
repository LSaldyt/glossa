cmake_minimum_required (VERSION 2.8.10)
cmake_policy(VERSION 2.8.10)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        message(FATAL_ERROR "GCC version must be at least 4.9. Sorry. I might make one that works on c++11 but not right now.")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.8)
        message(FATAL_ERROR "Clang version must be at least 3.8. Sorry. I might make one that works on c++11 but not right now.")
    endif()
else()
    message (WARNING "You are using an unsupported compiler. GCC is recommended for this program, even on Windows. If you do have any errors, do leave a report though")
endif()


if ("$ENV{TESTING}" STREQUAL "TRUE")
    set(PROJECT_NAME progtrantest)
    file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/src/*/*.cpp" # Skip Compiler.hpp and Compiler.cpp, which include int main()
        "${CMAKE_SOURCE_DIR}/src/*/*.hpp"
        "${CMAKE_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/*.hpp")
else()
    set(PROJECT_NAME progtran)
    file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.hpp")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

