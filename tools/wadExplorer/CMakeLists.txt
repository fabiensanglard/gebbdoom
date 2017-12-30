cmake_minimum_required(VERSION 3.6)
project(wadExplorer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-conversion -Wno-missing-braces -Wno-c++11-narrowing")

set(SOURCE_FILES
        main.cpp
        WadArchive.cpp
        WadArchive.h
        Lump.cpp
        Lump.h
        Map.cpp Map.h
        MapReader.cpp
        MapReader.h
        SVGRenderer.cpp
        SVGRenderer.h Things.h Things.cpp)
add_executable(wadExplorer ${SOURCE_FILES})