cmake_minimum_required(VERSION 3.14)
project(a3test)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(a3test
        Makefile
        mytest.c
        parse.c
        parse2.c
        pmake.c
        pmake.h
        run_make.c
        simpletest.c
        smalloc.c
        smalloc.h
        testhelpers.c)
