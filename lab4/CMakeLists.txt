cmake_minimum_required(VERSION 3.27)
project(Osisp)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(Osisp
        mydaemon.c
)
