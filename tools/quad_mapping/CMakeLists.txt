cmake_minimum_required(VERSION 3.10)
project(mapping)

set(CMAKE_CXX_STANDARD 11)

add_executable(mapping main.cpp targa.c targa.h)