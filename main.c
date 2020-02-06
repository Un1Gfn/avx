// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

// /usr/lib/gcc/x86_64-pc-linux-gnu/9.2.0/include/x86intrin.h
#include <stdio.h>
#include <sys/times.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "benchmark.h"
#include "dgemm_avx.h"

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

// void print(const double *m){
//   for(int r=0;r<n;++r){
//     for(int c=0;c<n;++c)
//       printf("%f ",((double (*)[n])m)[r][c]);
//     printf("\n");
//   }
// }

// void printall(){
//   print(A);printf("\n");
//   print(B);printf("\n");
//   print(C);printf("\n");
// }

void resize(){
  // n=n0;
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

void end(){
  free(A);A=NULL;
  free(B);B=NULL;
  free(C);C=NULL;
}

typedef struct{
  clock_t noavx;
  clock_t avx;
}Calctime;

Calctime test(int n0){

  if(n0==0)
    return (Calctime){};
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

  return (Calctime){.noavx=noavx,.avx=diff};

  end();

}

int main(){

  int xmin=0;
  int xmax=2000;
  int xn=20;

  #define ceil(x) ((x+3)/4*4)
  for(int x=ceil(xmin);x<=ceil(xmax);x+=(xmax-xmin+1)/xn){
    Calctime calctime=test(x);
    printf("%d %zu %zu\n",x,calctime.noavx,calctime.avx);
  }

  return 0;
}
