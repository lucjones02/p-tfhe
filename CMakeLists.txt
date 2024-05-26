cmake_minimum_required(VERSION 3.20)
project(fyp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++2a -Wall")

add_subdirectory(src)
add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main tfhe)
