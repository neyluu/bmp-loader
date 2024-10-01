cmake_minimum_required(VERSION 3.28)
project(bmp_to_ascii)

set(CMAKE_CXX_STANDARD 26)

add_executable(bmp_to_ascii
        main.cpp
        bmp.cpp
        bmp.h
)
