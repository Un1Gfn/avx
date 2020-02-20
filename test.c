#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
void f(){
  double *H=calloc(4096,sizeof(double));
  __m256d c0 = _mm256_load_pd(H);
  free(H);
}
int main(int argc,char *argv[]){
  fopen("somefile","wb");
  for(int x=0;x<1;++x){
    f();
  }
  return 0;
}

// $ cat /etc/lsb-release 
// DISTRIB_ID=Ubuntu
// DISTRIB_RELEASE=19.10
// DISTRIB_CODENAME=eoan
// DISTRIB_DESCRIPTION="Ubuntu 19.10"

// $ uname -a
// Linux d7e0ebba12c0 4.15.0-1052-aws #54-Ubuntu SMP Tue Oct 1 15:43:26 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux

// $ gcc --version
// gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Copyright (C) 2019 Free Software Foundation, Inc.
// This is free software; see the source for copying conditions.  There is NO
// warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// $ gcc -std=gnu11 -g -O0 -Wall -Wextra -Wno-unused-parameter -mavx test.c

// $ valgrind ./a.out
// ==15182== Memcheck, a memory error detector
// ==15182== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
// ==15182== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
// ==15182== Command: ./a.out
// ==15182== 
// ==15182== 
// ==15182== Process terminating with default action of signal 11 (SIGSEGV): dumping core
// ==15182==  General Protection Fault
// ==15182==    at 0x1091DC: _mm256_load_pd (avxintrin.h:862)
// ==15182==    by 0x1091DC: f (test.c:6)
// ==15182==    by 0x109232: main (test.c:12)
// ==15182== 
// ==15182== HEAP SUMMARY:
// ==15182==     in use at exit: 33,256 bytes in 2 blocks
// ==15182==   total heap usage: 2 allocs, 0 frees, 33,256 bytes allocated
// ==15182== 
// ==15182== LEAK SUMMARY:
// ==15182==    definitely lost: 0 bytes in 0 blocks
// ==15182==    indirectly lost: 0 bytes in 0 blocks
// ==15182==      possibly lost: 0 bytes in 0 blocks
// ==15182==    still reachable: 33,256 bytes in 2 blocks
// ==15182==         suppressed: 0 bytes in 0 blocks
// ==15182== Rerun with --leak-check=full to see details of leaked memory
// ==15182== 
// ==15182== For lists of detected and suppressed errors, rerun with: -s
// ==15182== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
// Segmentation fault
