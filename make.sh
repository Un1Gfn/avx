#!/bin/bash

CC='gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter'

$CC -mavx -c dgemm_avx.c

$CC $(pkg-config --cflags --libs gsl plplot glib-2.0) dgemm_avx.o main.c benchmark.c stats.c run.c
