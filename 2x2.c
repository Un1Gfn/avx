// D=/usr/share/plplot5.15.0/examples/c; gcc -H -std=gnu11 -g -O0 -Wall -Wextra -fdiagnostics-color=always -I"$D" -lm $(pkg-config --cflags --libs plplot) 2x2.c -o 2x2.out
// D=/usr/share/plplot5.15.0/examples/c; gcc -std=gnu11 -g -O0 -Wall -Wextra -fdiagnostics-color=always -I"$D" -lm $(pkg-config --cflags --libs plplot) 2x2.c -o 2x2.out
// unset -v D

#include "plcdemos.h"
#include <plplot/plevent.h>
#ifdef PL_HAVE_NANOSLEEP
#include <time.h>
#endif
#ifdef PL_HAVE_UNISTD_H
# include <unistd.h>
#endif

// Variables and data arrays used by plot generators
static PLFLT        x[101], y[101];
static PLFLT        xscale, yscale, xoff, yoff, xs[6], ys[6];

PLCHAR_VECTOR        notes[] = { "Make sure you get it right!", NULL };

// Function prototypes
void plot1();
void plot2();
void plot3();

//--------------------------------------------------------------------------
// main
//
// Generates several simple line plots.  Demonstrates:
//   - subwindow capability
//   - setting up the window, drawing plot, and labelling
//   - changing the color
//   - automatic axis rescaling to exponential notation
//   - placing the axes in the middle of the box
//   - gridded coordinate axes
//--------------------------------------------------------------------------

int main(){

  PLINT digmax=0;

  plsdev("xcairo");
  // plsdev("xwin");
  // plsdev("qtwidget");

  // https://www.x.org/releases/X11R7.7/doc/man/man7/X.7.xhtml#heading7
  // https://en.wikipedia.org/wiki/Display_resolution
  plsetopt("geometry","800x600");
  // plsetopt("geometry","+100+100");
  // plsetopt("geometry","800x600+10+10");
  // plsetopt("geometry","800x600+100+100");
  // plsetopt("geometry","1024x768+10+10");
  // plsetopt("geometry","1280x800");

  // plstar(2,2);

  plssub(2,2);
  plinit();

  // plfontld(0);

  // Set up the data
  // Original case
  xscale = 6.;
  yscale = 1.;
  xoff   = 0.;
  yoff   = 0.;
  // Do a plot
  plot1();

  plot1();
  // plot1();
  // plot1();

  plend();
  exit(0);

  // Set up the data
  // xscale = 1.;
  // yscale = 0.0014;
  // yoff   = 0.0185;
  // // Do a plot
  // digmax = 5;
  // plsyax( digmax, 0 );
  // plot1();
  // plot2();
  // plot3();

  // Clean up
  plend();
  exit( 0 );
}

//--------------------------------------------------------------------------

void plot1(){
  int   i;
  PLFLT xmin, xmax, ymin, ymax;

  for ( i = 0; i < 60; i++ ){
    x[i] = xoff + xscale * ( i + 1 ) / 60.0;
    y[i] = yoff + yscale * pow( x[i], 2. );
  }

  xmin = x[0];
  xmax = x[59];
  ymin = y[0];
  ymax = y[59];

  for ( i = 0; i < 6; i++ ){
      xs[i] = x[i * 10 + 3];
      ys[i] = y[i * 10 + 3];
  }

  // Set up the viewport and window using PLENV. The range in X is
  // 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
  // scaled separately (just = 0), and we just draw a labelled
  // box (axis = 0).
  // plcol0( 1 );
  plenv( xmin, xmax, ymin, ymax, 0, 0 );
  // plcol0( 2 );
  pllab( "(x)", "(y)", "#frPLplot Example 1 - y=x#u2" );

  // Plot the data points
  // plcol0( 4 );
  plpoin( 6, xs, ys, 9 );

  // Draw the line through the data
  // plcol0( 3 );
  plline( 60, x, y );

  plenv( xmin, xmax, ymin, ymax, 0, 0 );

}

//--------------------------------------------------------------------------

void plot2(){
  int i;

  // Set up the viewport and window using PLENV. The range in X is -2.0 to
  // 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
  // (just = 0), and we draw a box with axes (axis = 1).
  //
  plcol0( 1 );
  plenv( -2.0, 10.0, -0.4, 1.2, 0, 1 );
  plcol0( 2 );
  pllab( "(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function" );

  // Fill up the arrays

  for ( i = 0; i < 100; i++ ){
    x[i] = ( i - 19.0 ) / 6.0;
    y[i] = 1.0;
    if ( x[i] != 0.0 )
      y[i] = sin( x[i] ) / x[i];
  }

  // Draw the line

  plcol0( 3 );
  plwidth( 2 );
  plline( 100, x, y );
  plwidth( 1 );
}

//--------------------------------------------------------------------------

void plot3(){
  PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
  int   i;

  // For the final graph we wish to override the default tick intervals, and
  // so do not use plenv().
  pladv( 0 );

  // Use standard viewport, and define X range from 0 to 360 degrees, Y range
  // from -1.2 to 1.2.
  plvsta();
  plwind( 0.0, 360.0, -1.2, 1.2 );

  // Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.
  plcol0( 1 );
  plbox( "bcnst", 60.0, 2, "bcnstv", 0.2, 2 );

  // Superimpose a dashed line grid, with 1.5 mm marks and spaces.
  // plstyl expects a pointer!
  plstyl( 1, &mark1, &space1 );
  plcol0( 2 );
  plbox( "g", 30.0, 0, "g", 0.2, 0 );
  plstyl( 0, &mark0, &space0 );

  plcol0( 3 );
  pllab( "Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function" );

  for ( i = 0; i < 101; i++ )
  {
    x[i] = 3.6 * i;
    y[i] = sin( x[i] * M_PI / 180.0 );
  }

  plcol0( 4 );
  plline( 101, x, y );
}
