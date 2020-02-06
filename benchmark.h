// benchmark.h

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h> // clock_t

void start_clock();
void end_clock();
extern clock_t diff;

#endif