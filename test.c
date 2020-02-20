// gcc -std=gnu11 -g -O0 -Wall -Wextra -Wno-unused-parameter -mavx test.c

#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <string.h>

void f(){

  // double *H=calloc(4096,sizeof(double));

  // https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm256_load_pd&expand=3323
  // Load 256-bits (composed of 4 packed double-precision (64-bit) floating-point elements)
  // dst. mem_addr must be aligned on a 32-byte boundary or a general-protection exception may be generated.

  #define zero(p) memset(p,0,sizeof(double)*4)
  #define alloc aligned_alloc(32,sizeof(double)*4)

  double *a=alloc;
  zero(a);
  a[0]=1.;
  a[1]=2.;
  a[2]=3.;
  a[3]=4.;
  __m256d A=_mm256_load_pd(a);

  double *b=alloc;
  zero(b);
  b[0]=5.;
  b[1]=6.;
  b[2]=7.;
  b[3]=8.;
  __m256d B=_mm256_load_pd(b);

  __m256d S=_mm256_add_pd(A,B);
  double *s=alloc;
  zero(s);
  _mm256_store_pd(s,S);

  printf("%lf %lf %lf %lf\n",
    s[0],
    s[1],
    s[2],
    s[3]
  );

  free(a);
  free(b);
  free(s);
  a=b=s=NULL;

}

int main(int argc,char *argv[]){
  fopen("somefile","wb");
  for(int x=0;x<1;++x){
    f();
  }
  return 0;
}
