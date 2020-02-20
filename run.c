// dgemm.c

#include <assert.h>
#include <stdlib.h> // calloc()
#include <stdio.h> // fwrite()
#include <string.h> // memset()
#include <signal.h>
#include <x86intrin.h>
// #include <malloc.h> // malloc_usable_size()

#include "benchmark.h"
#include "record.h"
#include "run.h"

int n=0;
double *A=NULL;
double *B=NULL;

static FILE *f=NULL;
static char filename[100]={};

void handler(int i){
  fclose(f);
  f=NULL;
  remove(filename);
  printf("%s removed\n",filename);
  printf("SIGILL %d %d\n",i,SIGILL);
  signal(SIGILL,NULL); // Break out from recursion
  // raise(SIGILL);
}

static clock_t dgemm(){
  double *C=calloc(n*n,sizeof(double));;
  start_clock();
  for(int i=0;i<n;++i)
    for(int j=0;j<n;++j){
      double cij=C[i+j*n];/* cij=C[i][j] */
      for(int k=0;k<n;k++)
        cij += A[i+k*n] * B[k+j*n];/* cij += A[i][k]*B[k][j] */
      C[i+j*n]=cij;/* C[i][j]=cij */
    }
  end_clock();
  free(C);
  C=NULL;
  return diff;
}

// square n*n matrix
// n=0,4,8,...
clock_t dgemm_avx(){
  double *C=calloc(n*n,sizeof(double));;
  start_clock();
  for(int i=0;i<n;i+=4){
    for(int j=0;j<n;j++){
      printf("%d %d %d\n",i,j,n);
      __m256d c0=_mm256_load_pd(C+i+j*n);/* c0=C[i][j] */
      for(int k=0;k<n;k++)
      // c0=c0+A[i][k]*B[k][j] 
      c0=_mm256_add_pd(
        c0,
        _mm256_mul_pd(
          _mm256_load_pd(A+i+k*n),
          _mm256_broadcast_sd(B+k+j*n)
        )
      );
      // C[i][j]=c0
      _mm256_store_pd(C+i+j*n,c0);
    }
  }
  end_clock();
  free(C);
  C=NULL;
  return diff;
}

void test(int n0){

  double *H=calloc(4096,sizeof(double));
  __m256d c0 = _mm256_load_pd(H);
  free(H);

  // return (Record){};

  // if(n0==0)
  //   return (Record){};
  // assert(n0%4==0);
  // n=n0;
  // A=calloc(n*n,sizeof(double));
  // B=calloc(n*n,sizeof(double));
  // int val=0;
  // for(int i=0;i<2;++i){
  //   for(int r=0;r<n;++r){
  //     for(int c=0;c<n;++c){
  //       ((double (*)[n])((double*[2]){A,B}[i]))[r][c]=val;
  //       ++val;
  //     }
  //   }
  // }
  // Record r=(Record){n0,dgemm(),dgemm_avx()};
  // free(A);A=NULL;
  // free(B);B=NULL;
  // return r;

  // static int warned=0;
  // if(getenv("AVX")||getenv("avx")){
  //   // Remote execution
  //   if(!warned){
  //     printf("$AVX or $avx detected - bringing up dgemm_avx()\n");
  //     warned=1;
  //   }
  //   start_clock();
  //   dgemm_avx();
  //   end_clock();
  //   r.avx=diff;
  // }else{
  //   // Simulate
  //   if(!warned){
  //     printf("simulation ongoing - please set environment variable $AVX or $avx\n");
  //     warned=1;
  //   }
  //   start_clock();
  //   dgemm();
  //   end_clock();
  //   r.avx=diff*0.75;
  // }

}

void run(const char *fn,int xmin,int xmax,int xn){

  strcpy(filename,fn);
  f=fopen(filename,"wb");
  #define ceil(x) ((x+3)/4*4)
  for(int x=ceil(xmin);x<=ceil(xmax);x+=ceil((xmax-xmin+1)/xn)){
    // Record r=test(x);
    Record r=(Record){};
    test(x);
    record_show(r);
    fwrite(&r,sizeof(Record),1,f);
    fflush(f);
  }
  fclose(f);
  f=NULL;
  printf("%s\n",filename);
}
