// stats.c

#include <stdio.h> // fread()
#include <assert.h>
#include <gsl/gsl_fit.h> // gsl_fit_linear
#include <plplot/plplot.h>
#include <gmodule.h> // GArray
#include <math.h> // cbrt
#include "record.h"

typedef struct{
  double c0;
  double c1;
}Coefficients;

static Coefficients linear_regression(size_t n,double *x,double *y){

  double c0=0;
  double c1=0;
  double cov00=0;
  double cov01=0;
  double cov11=0;
  double sumsq=0;

  // https://www.gnu.org/software/gsl/doc/html/lls.html#c.gsl_fit_linear
  assert(gsl_fit_linear(x,1,y,1,n,&c0,&c1,&cov00,&cov01,&cov11,&sumsq)==0);
  return (Coefficients){c0,c1};

}

void stats(const char* filename){

  FILE *f=fopen(filename,"rb");

  GArray *x=g_array_new(FALSE,TRUE,sizeof(double));
  GArray *y_noavx=g_array_new(FALSE,TRUE,sizeof(double));
  GArray *y_avx=g_array_new(FALSE,TRUE,sizeof(double));
  Record r=(Record){};
  size_t n=0;

  // https://gcc.gnu.org/onlinedocs/gcc/Typeof.html
  // #define g_array_append_val_noref(a,v) {__typeof__(v) _v=v;g_array_append_val(a,_v);}
  #define g_array_append_val_noref(a,v) {__auto_type _v=v;g_array_append_val(a,_v);}

  while(fread(&r,sizeof(Record),1,f)){
    record_show(r);
    g_array_append_val_noref(x,(double)r.size);
    g_array_append_val_noref(y_noavx,cbrt((double)r.noavx));
    g_array_append_val_noref(y_avx,cbrt((double)r.avx));
    ++n;
    r=(Record){};
  }

  plsdev("qtwidget");
  plsetopt("geometry","+100+100");
  plinit();

  #define index(a,i) g_array_index(a,double,i)
  const PLFLT xmin=index(x,0);
  const PLFLT xmax=index(x,x->len-1)*1.05;
  const PLFLT ymin=index(y_noavx,0);
  const PLFLT ymax=index(y_noavx,y_noavx->len-1)*1.05;

  plenv(xmin,xmax,ymin,ymax,0,0);
  pllab("n*n","cube root of ticks","noavx");

  plpoin(n,(double*)x->data,(double*)y_noavx->data,23);

  Coefficients coefficients=linear_regression(n,(double*)(x->data),(double*)(y_noavx->data));
  #define y(x) (coefficients.c0+coefficients.c1*x)
  pljoin(xmin,y(xmin),xmax,y(xmax));

  plend();

  g_array_free(x,TRUE);
  g_array_free(y_noavx,TRUE);
  g_array_free(y_avx,TRUE);

}