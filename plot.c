// plot.c

// gcc -std=c11 -g -O0 -Wall -Wextra -Winline -fdiagnostics-color=always $(pkg-config --cflags --libs plplot) \
//   -I/usr/share/plplot5.15.0/examples/c/ \
//   /usr/share/plplot5.15.0/examples/c/x00c.c
// ./a.out -dev qtwidget -geometry +100+100
//  

// gcc -std=c11 -g -O0 -Wall -Wextra -Winline -fdiagnostics-color=always $(pkg-config --cflags --libs plplot) plot.c  

#include <plplot/plplot.h>

int main(int argc,char *argv[]){

  double x[]={
   23,
   26,
   30,
   34,
   43,
   48,
   52,
   57,
   58
  };
  double y[]={
    651,
    762,
    856,
    1063,
    1190,
    1298,
    1421,
    1440,
    1518
  };

  plsdev("qtwidget");
  plsetopt("geometry","+100+100");

  plinit();

  plenv(20,60,600,1600,0,0);
  pllab("Advertising","Sales","Scatter plot");

  plpoin(9,x,y,23);

  plend();

  return 0;
}