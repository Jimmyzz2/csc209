#!/usr/bin/env bash
./echo_arg csc209 > echo_out.txt
./echo_stdin < echo_stdin.c
./count 210 | wc
ls -S | head -1 | ./echo_stdin
