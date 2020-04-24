cmake_minimum_required(VERSION 3.14)
project(lab10 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab10
        bufserver.c
        randclient.c
        readserver.c
        socket.c
        socket.h)
