// stats.c

// gcc -std=c11 -g -O0 -Wall -Wextra -Winline -fdiagnostics-color=always $(pkg-config --cflags --libs plplot) \
//   -I/usr/share/plplot5.15.0/examples/c/ \
//   /usr/share/plplot5.15.0/examples/c/x00c.c
// ./a.out -dev qtwidget -geometry +100+100
//  

// gcc -std=c11 -g -O0 -Wall -Wextra -Winline -fdiagnostics-color=always $(pkg-config --libs gsl plplot) stats.c

#include <stdio.h>
#include <assert.h>
#include <gsl/gsl_fit.h>
#include <plplot/plplot.h>
#include "stats.h"

Coefficients linear_regression(size_t n,double *x,double *y){

  double c0=0;
  double c1=0;
  double cov00=0;
  double cov01=0;
  double cov11=0;
  double sumsq=0;

  // https://www.gnu.org/software/gsl/doc/html/lls.html#c.gsl_fit_linear
  assert(gsl_fit_linear(x,1,y,1,n,&c0,&c1,&cov00,&cov01,&cov11,&sumsq)==0);

  // printf("%lf %lf\n",c0,c1);
  // printf("%lf %lf %lf\n",cov00,cov01,cov11);
  // printf("%lf\n",sumsq);
  return (Coefficients){c0,c1};

}

// int main()
// {

//   double x[]={
//    23,
//    26,
//    30,
//    34,
//    43,
//    48,
//    52,
//    57,
//    58
//   };
//   double y[]={
//     651,
//     762,
//     856,
//     1063,
//     1190,
//     1298,
//     1421,
//     1440,
//     1518
//   };
//   size_t n=9;

//   plsdev("qtwidget");
//   plsetopt("geometry","+100+100");
//   plinit();

//   const PLFLT xmin=20;
//   const PLFLT xmax=60;
//   const PLFLT ymin=600;
//   const PLFLT ymax=1600;
//   plenv(xmin,xmax,ymin,ymax,0,0);
//   pllab("Advertising","Sales","Scatter plot");

//   plpoin(n,x,y,23);

//   Coefficients coefficients=linear_regression(n,x,y);
//   #define y(x) (coefficients.c0+coefficients.c1*x)
//   pljoin(xmin,y(xmin),xmax,y(xmax));

//   plend();

//   return 0;
// }