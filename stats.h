#ifndef STATS_H
#define STATS_H

typedef struct{
  double c0;
  double c1;
}Coefficients;

Coefficients linear_regression(size_t n,double *x,double *y);

#endif