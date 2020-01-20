// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

// gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter -fmax-errors=1 matrix.c
// gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter -mavx -S matrix.c
// gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter -mavx matrix.c

// /usr/lib/gcc/x86_64-pc-linux-gnu/9.2.0/include/x86intrin.h
#include <x86intrin.h>
#include <stdio.h>

void dgemm(int n,double *A,double *B,double *C){
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

int main(){
  // double a[1][1]={{12.34}};
  // double b[1][1]={{56.78}};
  // double c[1][1]={{}};
  double a[1]={12.34};
  double b[1]={56.78};
  double c[1]={};
  dgemm(1,a,b,c);
  printf("%f\n",c[0]);
  return 0;
}
