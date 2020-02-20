// dgemm_avx.c

// /usr/lib/gcc/x86_64-pc-linux-gnu/9.2.0/include/x86intrin.h
#include <x86intrin.h>

extern int n;
extern double *A;
extern double *B;
extern double *C;

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
