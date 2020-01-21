#!/bin/bash

CC='gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter'

$CC -mavx -c dgemm_avx.c

$CC dgemm_avx.o main.c benchmark.c
