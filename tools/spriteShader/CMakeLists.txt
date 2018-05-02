cmake_minimum_required(VERSION 3.10)
project(spriteShader)

set(CMAKE_CXX_STANDARD 11)

add_executable(spriteShader
        main.cpp
        Palette.cpp
        Color.cpp
        Colormap.cpp
        targa.c
        Sprite.cpp
)