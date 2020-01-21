// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

// gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter -mavx main.c benchmark.c

// /usr/lib/gcc/x86_64-pc-linux-gnu/9.2.0/include/x86intrin.h
#include <x86intrin.h>
#include <stdio.h>
#include <sys/times.h>
#include <string.h>
#include "benchmark.h"

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

// square n*n matrix
// n=0,4,8,...
void dgemm_avx(){
  for(int i=0;i<n;i+=4){
    for(int j=0;j<n;j++){
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
}

void print(const double *m){
  for(int r=0;r<n;++r){
    for(int c=0;c<n;++c)
      printf("%f ",((double (*)[n])m)[r][c]);
    printf("\n");
  }
}

void init(){
  n=4;
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

int main(){

  init();
  print(A);printf("\n");
  print(B);printf("\n");
  print(C);printf("\n");
  printf("\n");

  dgemm();
  print(A);printf("\n");
  print(B);printf("\n");
  print(C);printf("\n");
  printf("\n");

  end();

  return 0;
}
