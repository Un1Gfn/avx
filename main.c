// main.c

// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

#include <stdio.h>
#include <sys/times.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <gmodule.h>
#include <gsl/gsl_fit.h>
#include <plplot/plplot.h>

#include "benchmark.h"
#include "dgemm_avx.h"
#include "stats.h"

#define ERR {printf("ERR %s %d\n",__FILE__,__LINE__);exit(1);}

int n=0;
double *A=NULL;
double *B=NULL;
double *C=NULL;

void dgemm(){
  for(int i=0;i<n;++i)
    for(int j=0;j<n;++j){
      double cij=C[i+j*n];/* cij=C[i][j] */
      for(int k=0;k<n;k++)
        cij += A[i+k*n] * B[k+j*n];/* cij += A[i][k]*B[k][j] */
      C[i+j*n]=cij;/* C[i][j]=cij */
    }
}

void resize(){
  A=calloc(n*n,sizeof(double));
  B=calloc(n*n,sizeof(double));
  C=calloc(n*n,sizeof(double));
  int val=0;
  for(int i=0;i<2;++i){
    for(int r=0;r<n;++r){
      for(int c=0;c<n;++c){
        ((double (*)[n])((double*[2]){A,B}[i]))[r][c]=val;
        ++val;
      }
    }
  }
}

void clearC(){
  memset((void*)C,0,n*n*sizeof(double));
}

void dealloc(){
  free(A);A=NULL;
  free(B);B=NULL;
  free(C);C=NULL;
}

typedef struct{
  int size;
  clock_t noavx;
  clock_t avx;
}Record;
#define record_show(r) printf("%d %zu %zu\n",r.size,r.noavx,r.avx)

Record test(int n0){

  if(n0==0)
    return (Record){};
  assert(n0%4==0);

  n=n0;
  resize();

  start_clock();
  dgemm();
  end_clock();
  clock_t noavx=diff;

  clearC();

  start_clock();
  // dgemm_avx();
  end_clock();

  return (Record){.size=n,.noavx=noavx,.avx=diff};

  dealloc();

}

int main(int argc,char *argv[]){

  if(argc<2)
    ERR

  if(strcmp(argv[1],"run")==0){

    FILE *f=fopen(argv[2],"wb");
    int xmin=atoi(argv[3]);
    int xmax=atoi(argv[4]);
    int xn=atoi(argv[5]);

    #define ceil(x) ((x+3)/4*4)
    for(int x=ceil(xmin);x<=ceil(xmax);x+=ceil((xmax-xmin+1)/xn)){
      Record r=test(x);
      record_show(r);
      fwrite(&r,sizeof(Record),1,f);
      fflush(f);
    }

    fclose(f);
    return 0;

  }

  if(strcmp(argv[1],"stat")==0){

    FILE *f=fopen(argv[2],"rb");

    GArray *x=g_array_new(FALSE,TRUE,sizeof(double));
    GArray *y_noavx=g_array_new(FALSE,TRUE,sizeof(double));
    GArray *y_avx=g_array_new(FALSE,TRUE,sizeof(double));
    Record r=(Record){};
    size_t n=0;

    while(fread(&r,sizeof(Record),1,f)){
      record_show(r);
      double tmp=0;
      tmp=(double)r.size;
      g_array_append_val(x,tmp);
      tmp=cbrt((double)r.noavx);
      g_array_append_val(y_noavx,tmp);
      tmp=cbrt((double)r.avx);
      g_array_append_val(y_avx,tmp);
      ++n;
      r=(Record){};
    }

    // plsdev("xwin");
    // plsdev("tk");
    // plsdev("ntk");
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

    return 0;

  }

  ERR;

}
