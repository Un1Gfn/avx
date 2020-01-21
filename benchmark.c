// benchmark.c

// times(2)

// times(3p)
/* This example assumes that the result of each subtraction
  is within the range of values that can be represented in
  an integer type. */

#include <stdint.h>
#include <stdio.h>
// #include <time.h> // clock_t
#include <sys/times.h>

static clock_t st_time=0;
static clock_t en_time=0;

static struct tms st_cpu=(struct tms){};
static struct tms en_cpu=(struct tms){};

void start_clock(){
  st_time=0;
  en_time=0;
  st_cpu=(struct tms){};
  en_cpu=(struct tms){};
  st_time = times(&st_cpu);
}

void end_clock(const char *msg)
{
  en_time=times(&en_cpu);
  if(msg)
    fputs(msg,stdout);
  printf("Real Time: %jd, User Time %jd, System Time %jd\n",
    (intmax_t)(en_time - st_time),
    (intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime),
    (intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime)
  );
}
