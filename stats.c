// stats.c
// gcc -std=c11 -g -O0 -Wall -Wextra -Winline -fdiagnostics-color=always $(pkg-config --libs gsl) stats.c

#include <stdio.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_fit.h>

int main()
{

  double x[]={
   23,
   26,
   30,
   34,
   43,
   48,
   52,
   57,
   58
  };
  double y[]={
    651,
    762,
    856,
    1063,
    1190,
    1298,
    1421,
    1440,
    1518
  };

  double c0=0;
  double c1=0;
  double cov00=0;
  double cov01=0;
  double cov11=0;
  double sumsq=0;

  int r=gsl_fit_linear(x,1,y,1,9,&c0,&c1,&cov00,&cov01,&cov11,&sumsq);
  
  printf("%d %lf %lf\n",r,c0,c1);
  printf("%lf %lf %lf\n",cov00,cov01,cov11);
  printf("%lf\n",sumsq);

  return 0;
}

// int main()
// {
//   double x=5.0;
//   double y=gsl_sf_bessel_J0 (x);
//   printf("J0(%g) = %.18e\n",x,y);
//   return 0;
// }