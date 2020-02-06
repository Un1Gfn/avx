// main.c

// Subword Parallelism - SSE - XMM

// /home/darren/pdf/Computer-Organization-and-Design-The-Hardware-Software-Interface-5th-Edition-With-all-appendices-and-advanced-material.pdf

// #include <string.h>
#include <stdio.h> // ERR
#include <stdlib.h> // atoi()
#include <string.h> // strcmp
#include "stats.h"
#include "run.h"

#define ERR {printf("ERR %s %d\n",__FILE__,__LINE__);exit(1);}

int main(int argc,char *argv[]){

  if(argc<2)
    ERR;

  if(strcmp(argv[1],"run")==0){
    if(argc!=6)
      ERR
    run(
      argv[2],
      atoi(argv[3]),
      atoi(argv[4]),
      atoi(argv[5])
    );
    return 0;
  }

  if(strcmp(argv[1],"stats")==0){
    if(argc!=3)
      ERR;
    stats(argv[2]);
    return 0;
  }

  ERR;

}
