// dgemm.c

#include <assert.h>
#include <stdlib.h> // calloc()
#include <stdio.h> // fwrite()
#include <string.h> // memset()
#include "dgemm_avx.h"
#include "benchmark.h"
#include "record.h"

int n=0;
double *A=NULL;
double *B=NULL;
double *C=NULL;

static void dgemm(){
  for(int i=0;i<n;++i)
    for(int j=0;j<n;++j){
      double cij=C[i+j*n];/* cij=C[i][j] */
      for(int k=0;k<n;k++)
        cij += A[i+k*n] * B[k+j*n];/* cij += A[i][k]*B[k][j] */
      C[i+j*n]=cij;/* C[i][j]=cij */
    }
}

static void resize(){
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

static void clearC(){
  memset((void*)C,0,n*n*sizeof(double));
}

static void dealloc(){
  free(A);A=NULL;
  free(B);B=NULL;
  free(C);C=NULL;
}

static Record test(int n0){

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

void run(const char *filename,int xmin,int xmax,int xn){

  FILE *f=fopen(filename,"wb");
  
  #define ceil(x) ((x+3)/4*4)
  for(int x=ceil(xmin);x<=ceil(xmax);x+=ceil((xmax-xmin+1)/xn)){
    Record r=test(x);
    record_show(r);
    fwrite(&r,sizeof(Record),1,f);
    fflush(f);
  }
  fclose(f);
}
