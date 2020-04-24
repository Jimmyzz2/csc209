cmake_minimum_required(VERSION 3.14)
project(lab4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab4
    compare.c
    greeting.c
    Makefile
    strip.c
    truncate.c)
