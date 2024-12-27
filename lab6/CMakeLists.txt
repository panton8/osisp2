cmake_minimum_required(VERSION 3.27)
project(Osisp6 C)

set(CMAKE_C_STANDARD 23)

add_executable(Osisp6
        server.c
        client.c)
