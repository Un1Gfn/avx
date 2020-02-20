// main.c

// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

#include <stdio.h> // HINT
#include <stdlib.h> // atoi()
#include <x86intrin.h>
// #include <string.h> // strcmp
// #include <signal.h>
// #include "stats.h"
// #include "run.h"

// #define HINT {printf("\n  %s run begin[0] end[1000] n[10]\n  %s stats <*.bin>\n\n",argv[0],argv[0]);exit(1);}

void test2(int n0){
  double *H=calloc(4096,sizeof(double));
  __m256d c0 = _mm256_load_pd(H);
  free(H);
}

int main(int argc,char *argv[]){

  FILE *f=fopen("somefile","wb");
  for(int x=0;x<1;++x){
    test2(0);
  }
  return 0;

  // signal(SIGILL,handler);
  // if(argc<2)
  //   HINT;
  // if(strcmp(argv[1],"run")==0){
  //   if(argc!=5)
  //     HINT;
  //   // http://www.cplusplus.com/reference/cstdio/snprintf/#example
  //   // result_0_1000_10.bin
  //   #define FN 100
  //   char filename[FN]={};
  //   snprintf(filename,FN,"result_%s_%s_%s.bin",argv[2],argv[3],argv[4]);
  //   // run(0);
  //   test(0);
  //   run(
  //     filename,
  //     atoi(argv[2]),
  //     atoi(argv[3]),
  //     atoi(argv[4])
  //   );
  //   return 0;
  // }
  // if(strcmp(argv[1],"stats")==0){
  //   if(argc!=3)
  //     HINT;
  //   stats(argv[2]);
  //   return 0;
  // }
  // HINT;
  // return 0;

}
