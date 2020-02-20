// dgemm.c

#include <assert.h>
#include <stdlib.h> // calloc()
#include <stdio.h> // fwrite()
#include <string.h> // memset()
#include <signal.h>
// #include <malloc.h> // malloc_usable_size()
#include "dgemm_avx.h"
#include "benchmark.h"
#include "record.h"

extern int n;
extern double *A;
extern double *B;
extern double *C;

static FILE *f=NULL;
static char filename[100]={};

static void dgemm(){
  for(int i=0;i<n;++i)
    for(int j=0;j<n;++j){
      double cij=C[i+j*n];/* cij=C[i][j] */
      for(int k=0;k<n;k++)
        cij += A[i+k*n] * B[k+j*n];/* cij += A[i][k]*B[k][j] */
      C[i+j*n]=cij;/* C[i][j]=cij */
    }
}

static void build_AB_init_C(){
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

static void clear_C(){
  memset((void*)C,0,n*n*sizeof(double));
}

static void dealloc_ABC(){
  free(A);A=NULL;
  free(B);B=NULL;
  free(C);C=NULL;
}

void handler(int i){
  fclose(f);
  f=NULL;
  remove(filename);
  printf("%s removed\n",filename);
  printf("SIGILL %d %d\n",i,SIGILL);
  signal(SIGILL,NULL); // Break out from recursion
  // raise(SIGILL);
}

static Record test(int n0){

  Record r=(Record){};

  if(n0==0)
    return r;
  assert(n0%4==0);

  n=n0;
  build_AB_init_C();
  r.size=n0;

  start_clock();
  dgemm();
  end_clock();
  r.noavx=diff;

  clear_C();

  static int warned=0;
  if(getenv("AVX")||getenv("avx")){
    // Remote execution
    if(!warned){
      printf("$AVX or $avx detected - bringing up dgemm_avx()\n");
      warned=1;
    }
    start_clock();
    dgemm_avx();
    end_clock();
    r.avx=diff;
  }else{
    // Simulate
    if(!warned){
      printf("simulation ongoing - please set environment variable $AVX or $avx\n");
      warned=1;
    }
    start_clock();
    dgemm();
    end_clock();
    r.avx=diff*0.75;
  }

  dealloc_ABC();
  return r;

}

void run(const char *fn,int xmin,int xmax,int xn){
  strcpy(filename,fn);
  f=fopen(filename,"wb");
  #define ceil(x) ((x+3)/4*4)
  for(int x=ceil(xmin);x<=ceil(xmax);x+=ceil((xmax-xmin+1)/xn)){
    Record r=test(x);
    record_show(r);
    fwrite(&r,sizeof(Record),1,f);
    fflush(f);
  }
  fclose(f);
  f=NULL;
  printf("%s\n",filename);
}
