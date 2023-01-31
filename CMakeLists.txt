
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Filter VERSION 1.0)

add_definitions(-std=c++20)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

add_executable(Filter main.cpp)

target_include_directories(Filter PUBLIC include)

target_sources(Filter PUBLIC
    src/process/Kernel.cpp
    src/image/Image.cpp)