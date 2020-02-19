#!/bin/bash

CC='gcc -std=gnu11 -g -O0 -Wall -Wextra -Wno-unused-parameter'

RUN="$CC -mavx -c dgemm_avx.c"
echo "$RUN"
$RUN

RUN="$CC $(pkg-config --cflags --libs gsl plplot glib-2.0) dgemm_avx.o main.c benchmark.c stats.c run.c"
echo "$RUN"
$RUN

exit 0
