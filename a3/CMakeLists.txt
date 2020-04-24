cmake_minimum_required(VERSION 3.14)
project(a3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(a3
        helpers.c
        Makefile
        parse.c
        pmake.c
        pmake.h
        run_make.c
        test.txt
        parse2.c
        ptest.c
        )

