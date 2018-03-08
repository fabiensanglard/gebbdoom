cmake_minimum_required(VERSION 3.10)
project(rowmajorize)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(SOURCE_FILES main.cpp)
add_executable(rowmajorize ${SOURCE_FILES})