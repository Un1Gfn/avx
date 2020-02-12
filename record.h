#ifndef RECORD_H
#define RECORD_H

#include <time.h>

typedef struct{
  int size; // size*size square matrix
  clock_t noavx;
  clock_t avx;
}Record;
#define record_show(r) printf("%d %zu %zu\n",r.size,r.noavx,r.avx)

#endif