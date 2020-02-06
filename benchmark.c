// benchmark.c

// times(2)
// times(3p)

#include <sys/times.h>

static clock_t begin=0;
static clock_t end=0;
static struct tms tms0=(struct tms){};

clock_t diff=0;

void start_clock(){
  begin=0;
  end=0;
  diff=0;
  tms0=(struct tms){};
  begin=times(&tms0);
}

void end_clock()
{
  tms0=(struct tms){};
  end=times(&tms0);
  diff=end-begin;
}
