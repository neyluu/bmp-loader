cmake_minimum_required(VERSION 3.27.8)
project(bmp_to_ascii)

set(CMAKE_CXX_STANDARD 26)

add_compile_options(
    -g
#    -pedantic
    -fno-elide-constructors
)

add_executable(bmp_to_ascii
        main.cpp

        bmp/bmp.cpp
        bmp/bmp.h
        bmp/utils.cpp
        bmp/load.cpp
        bmp/save.cpp

        tests/tests.cpp
        tests/tests.h
)
