// main.c

// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

// #include <string.h>
#include <stdio.h> // HINT
#include <stdlib.h> // atoi()
#include <string.h> // strcmp
#include "stats.h"
#include "run.h"

#define HINT {printf("\n  %s run begin[0] end[1000] n[10]\n  %s stats <*.bin>\n\n",argv[0],argv[0]);exit(1);}

int main(int argc,char *argv[]){

  if(argc<2)
    HINT;

  if(strcmp(argv[1],"run")==0){
    if(argc!=5)
      HINT;
    // http://www.cplusplus.com/reference/cstdio/snprintf/#example
    // result_0_1000_10.bin
    #define FN 100
    char filename[FN]={};
    snprintf(filename,FN,"result_%s_%s_%s.bin",argv[2],argv[3],argv[4]);
    run(
      filename,
      atoi(argv[2]),
      atoi(argv[3]),
      atoi(argv[4])
    );
    return 0;
  }

  if(strcmp(argv[1],"stats")==0){
    if(argc!=3)
      HINT;
    stats(argv[2]);
    return 0;
  }

  HINT;

}
