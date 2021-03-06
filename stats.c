// stats.c

#include <stdio.h> // fread()
#include <assert.h>
#include <gsl/gsl_fit.h> // gsl_fit_linear
#include <plplot/plplot.h>
#include <glib-2.0/gmodule.h> // GArray
#include <math.h> // cbrt
#include "record.h"

// #define g_array_append_val_noref(a,v) {__typeof__(v) _v=v;g_array_append_val(a,_v);}
#define g_array_append_val_noref(a,v) {__auto_type _v=v;g_array_append_val(a,_v);}
#define index_double(a,i) g_array_index(a,double,i)
#define g_array_new_double() g_array_new(FALSE,TRUE,sizeof(double))
#define g_array_sized_new_double(reserved_size) g_array_sized_new(FALSE,TRUE,sizeof(double),reserved_size)
#define g_array_free_dealloc(array) g_array_free(array,TRUE)
#define margin 0.05

static const double *x=NULL;
static PLINT len=0;

// https://www.gnu.org/software/gsl/doc/html/lls.html#c.gsl_fit_linear
// Y=c0+c1*X
typedef struct{
  double c0;
  double c1;
}Coefficients;

static Coefficients linear_regression(double *y_cbrt){
  double c0=0;
  double c1=0;
  double cov00=0;
  double cov01=0;
  double cov11=0;
  double sumsq=0;
  // https://www.gnu.org/software/gsl/doc/html/lls.html#c.gsl_fit_linear
  assert(gsl_fit_linear(x,1,y_cbrt,1,len,&c0,&c1,&cov00,&cov01,&cov11,&sumsq)==0);
  return (Coefficients){c0,c1};
}

static void plot2(const double* const y,const char *tlabel){

  // Cube roots
  double y_cbrt[len];
  memset(y_cbrt,0,len*sizeof(double));
  for(PLINT i=0;i<len;++i)
    y_cbrt[i]=cbrt(y[i]);

  // Regression coefficients
  Coefficients coefficients=linear_regression(y_cbrt);
  #define hat_cbrt(x) (coefficients.c0+coefficients.c1*x)
  #define hat(x) (hat_cbrt(x)*hat_cbrt(x)*hat_cbrt(x))

  const PLFLT xmin=x[0]*(1.0-margin);
  const PLFLT xmax=x[len-1]*(1.0+margin);

  // Cubic
  plenv(
    xmin,
    xmax,
    /*ymin*/ (PLFLT) y[0]*(1.0-margin),
    /*ymax*/ (PLFLT) y[len-1]*(1.0+margin),
    /*just*/ 0,
    /*axis*/ 0
  );
  pllab("matrix size","ticks",tlabel);
  plpoin(len,x,y,23);
  // y_hat=(c0+c1*x)^3
  double y_hat[len];
  memset(y_hat,0,len*sizeof(double));
  for(PLINT i=0;i<len;++i){
    y_hat[i]=hat(x[i]);
    // printf("%lf %lf\n",hat_cbrt(index_double(x,i)),hat(index_double(x,i)));
  }
  // plcol0( 3 );
  // plwidth(2);
  pljoin(xmin,hat(xmin),x[0],y_hat[0]);
  plline(len,x,y_hat);
  pljoin(x[len-1],y_hat[len-1],xmax,hat(xmax));
  // plpoin(len,(double*)x->data,y_hat,23);

  // Linear
  plenv(
    xmin,
    xmax,
    /*ymin*/ (PLFLT) y_cbrt[0]*(1.0-margin),
    /*ymax*/ (PLFLT) y_cbrt[len-1]*(1.0+margin),
    /*just*/ 0,
    /*axis*/ 0
  );
  pllab("matrix size","cube root of ticks",tlabel);
  plpoin(len,x,y_cbrt,23);
  pljoin(xmin,hat_cbrt(xmin),xmax,hat_cbrt(xmax));

}

void stats(const char* filename){

  // Init
  GArray *x0=g_array_new_double();
  GArray *y_noavx=g_array_new_double();
  GArray *y_avx=g_array_new_double();
  Record r=(Record){};
  plsdev("xcairo");
  plsetopt("geometry","+100+100");
  plssub(2,2);
  plinit();

  // Read file
  // https://gcc.gnu.org/onlinedocs/gcc/Typeof.html
  FILE *f=fopen(filename,"rb");
  while(fread(&r,sizeof(Record),1,f)){
    // record_show(r);
    g_array_append_val_noref(x0,(double)r.size);
    g_array_append_val_noref(y_noavx,(double)r.noavx);
    g_array_append_val_noref(y_avx,(double)r.avx);
    r=(Record){};
  }
  len=x0->len;
  x=(double*)x0->data;

  plot2((double*)y_noavx->data,"noavx");
  plot2((double*)y_avx->data,"avx");

  // Cleanup
  x=NULL;
  len=0;
  g_array_free_dealloc(x0);
  g_array_free_dealloc(y_noavx);
  g_array_free_dealloc(y_avx);
  plend();

}