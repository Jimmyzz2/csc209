cmake_minimum_required(VERSION 3.14)
project(lab11 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab11
        chat_client.c
        chat_server.c)
