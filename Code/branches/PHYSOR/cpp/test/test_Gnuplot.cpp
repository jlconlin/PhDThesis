/*test_Gnuplot.cpp
 $Author: jlconlin $
 $Id: test_Gnuplot.cpp 267 2008-03-05 16:52:05Z jlconlin $
 $Revision: 267 $
 $Date: 2008-03-05 09:52:05 -0700 (Wed, 05 Mar 2008) $

 This file is for testing the C++ interface to Gnuplot.
 */

// Example for C++ Interface to Gnuplot

// requirements:
// * gnuplot has to be installed (http://www.gnuplot.info/download.html)
// * set Path-variable for Gnuplot path (e.g. C:/program files/gnuplot/bin; /usr/bin)
//     or set Gnuplot path with: Gnuplot::set_GNUPlotPath(const string &path)


#include <iostream>
#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
 #include <conio.h> //for getch(), needed in wait_for_key()
 #include <windows.h> //for Sleep()
    void sleep(int i) { Sleep(i*1000); }
#endif


#define SLEEP_LGTH 1 // sleep time in seconds

#define NPOINTS    50 // length of array


using namespace std;


void wait_for_key(); // Programm halts until keypress



int main(int argc, char* argv[])
{
    // if path-variable for gnuplot is not set, do it with:
    // Gnuplot::set_GNUPlotPath("C:/program files/gnuplot/bin/");

    cout << "*** example of gnuplot control through C++ ***" << endl << endl;

    //
    // Using the GnuplotException class
    //
    try
      {
	Gnuplot g1 = Gnuplot("lines", false);

	//
	// Slopes
	//
	g1.set_title("Slopes");
	cout << "*** plotting slopes" << endl;
	cout << "y = x" << endl;
	g1.plot_slope(1.0,0.0,"unity slope");
	sleep(SLEEP_LGTH);

	cout << "y = 2*x" << endl;
	g1.plot_slope(2.0,0.0,"y=2x");
	sleep(SLEEP_LGTH);

	cout << "y = -x" << endl;
	g1.plot_slope(-1.0,0.0,"y=-x");
	sleep(SLEEP_LGTH);
        g1.unset_title();

        g1.reset_plot();
	cout << endl << endl << "*** save to ps " << endl;
	cout << "y = sin(x)" << endl;
	g1.savetops("test_output");
	g1.plot_equation("sin(x)","sine");
	g1.showonscreen();

	//
	// Equations
	//
	g1.reset_plot();
	cout << endl << endl << "*** various equations" << endl;
	cout << "y = sin(x)" << endl;
	g1.plot_equation("sin(x)","sine");
	sleep(SLEEP_LGTH);

	cout << "y = log(x)" << endl;
	g1.plot_equation("log(x)","logarithm");
	sleep(SLEEP_LGTH);

	cout << "y = sin(x) * cos(2*x)" << endl;
	g1.plot_equation("sin(x)*cos(2*x)","sine product");
	sleep(SLEEP_LGTH);

	//
	// Styles
	//
	g1.reset_plot();
	cout << endl << endl << "*** showing styles" << endl;
	cout << "sine in points" << endl;
	g1.unset_legend();
	g1.set_pointsize(0.8).set_style("points");
	g1.plot_equation("sin(x)","sine");
	sleep(SLEEP_LGTH);

	cout << "sine in impulses" << endl;
	g1.set_style("impulses");
	g1.plot_equation("sin(x)","sine");
	sleep(SLEEP_LGTH);

	cout << "sine in steps" << endl;
	g1.set_style("steps");
	g1.plot_equation("sin(x)","sine");
	sleep(SLEEP_LGTH);

	g1.set_legend();
	g1.set_pointsize();

	//
	// User defined 1d, 2d and 3d point sets
	//
	vector<double> x;
        vector<double> y;
        vector<double> z;
        vector<double> dy;

	for (int i = 0; i < NPOINTS; i++)  // fill double arrays x, y, z
	  {
	    x.push_back((double)i);             // x[i] = i
	    y.push_back((double)i * (double)i); // y[i] = i^2
	    z.push_back( x[i]*y[i] );           // z[i] = x[i]*y[i] = i^3
	    dy.push_back((double)i * (double)i / (double) 10); // dy[i] = i^2 / 10
	  }

	cout << endl << endl << "*** user-defined lists of doubles" << endl;
	g1.reset_plot();
	g1.set_style("impulses").plot_x(y,"user-defined doubles");
	sleep(SLEEP_LGTH);


        g1.set_grid();
	cout << endl << endl << "*** user-defined lists of points (x,y)" << endl;
	g1.reset_plot();
	g1.set_style("points").plot_xy(x,y,"user-defined points 2d");
	sleep(SLEEP_LGTH);


        g1.unset_grid();
	cout << endl << endl << "*** user-defined lists of points (x,y,z)" << endl;
	g1.reset_plot();
	g1.plot_xyz(x,y,z,"user-defined points 3d");
        sleep(SLEEP_LGTH);


	cout << endl << endl << "*** user-defined lists of points (x,y,dy)" << endl;
	g1.reset_plot();
	g1.plot_xy_err(x,y,dy,"user-defined points 2d with errorbars");
        sleep(SLEEP_LGTH);


	//
	// Multiple output screens
	//
	cout << endl << endl;
	cout << "*** multiple output windows" << endl;

	g1.reset_plot();
	g1.set_style("lines");

	Gnuplot g2 = Gnuplot("lines", false);
	Gnuplot g3 = Gnuplot("lines", false);

	cout << "window 1: sin(x)" << endl;
	g1.plot_equation("sin(x)","sin(x)");
	sleep(SLEEP_LGTH);

	cout << "window 2: x*sin(x)" << endl;
	g2.set_grid();
	g2.plot_equation("x*sin(x)","x*sin(x)");
	sleep(SLEEP_LGTH);

	cout << "window 3: log(x)/x" << endl;
	g3.plot_equation("log(x)/x","log(x)/x");
	sleep(SLEEP_LGTH);

	Gnuplot g4 = Gnuplot("lines", false);
        cout << "window 4: sin(x)/x" << endl;
        g4.plot_equation("sin(x)/x","sin(x)/x");
        sleep(SLEEP_LGTH);

	// if you use the default constructor you're forced to
	// to use the x11 terminal. If you want to use the aqua 
	// terminal, you should use: Gnuplot g5 = Gnuplot(false)
        Gnuplot g5 = Gnuplot(false) ;
        cout << "window 5: splot x*x+y*y" << endl;
        g5.set_zrange(0,100);
        g5.set_xlabel("x-axis").set_ylabel("y-axis").set_zlabel("z-axis");
        g5.cmd("splot x*x+y*y");
        sleep(SLEEP_LGTH);

        Gnuplot g6 = Gnuplot(false);
        cout << "window 6: splot x*y*y" << endl;
        g6.set_style("lines").set_isosamples(25).set_hidden3d();
        g6.plot_equation3d("x*y*y");
        sleep(SLEEP_LGTH);

        Gnuplot g7 = Gnuplot(false);
        cout << "window 7: set_samples(400)" << endl;
        g7.set_xrange(-30,20).set_style("lines").set_samples(400);
        g7.plot_equation("besj0(x)*0.12e1","impulses").plot_equation("(x**besj0(x))-2.5", "points");
        sleep(SLEEP_LGTH);

        Gnuplot g8("filledcurves", false);
        cout << "window 8: filledcurves" << endl;
        g8.set_legend("outside right top").set_xrange(-5,5).plot_equation("x*x").plot_equation("-x*x+4");

        //
        // Plot an image
        //
        Gnuplot g9 = Gnuplot(false);
        cout << "window 9: plot_image" << endl;
        const int iWidth  = 255;
        const int iHeight = 255;
        g9.set_xrange(0,iWidth).set_yrange(0,iHeight).set_cbrange(0,255);
        g9.cmd("set palette gray");
        unsigned char ucPicBuf[iWidth*iHeight];
        // generate a greyscale image
        for(int iIndex = 0; iIndex < iHeight*iWidth; iIndex++)
	  {
            ucPicBuf[iIndex] = iIndex%255;
	  }
        g9.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
        sleep(SLEEP_LGTH);

        g9.set_pointsize(0.6).unset_legend().plot_slope(0.8,20);
        sleep(SLEEP_LGTH);

        //
        // manual control
        //
        Gnuplot g10 = Gnuplot(false);
        cout << "window 10: manual control" << endl;
        g10.cmd("set samples 400").cmd("plot abs(x)/2"); // either with cmd()
        g10 << "replot sqrt(x)" << "replot sqrt(-x)";    // or with <<

	wait_for_key();

      }
    catch (GnuplotException ge)
      {
        cout << ge.what() << endl;
      }


    cout << endl << "*** end of gnuplot example" << endl;

    return 0;
}



void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
  cout << endl << "Press any key to continue..." << endl;

  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
  _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__)
  cout << endl << "Press ENTER to continue..." << endl;

  std::cin.clear();
  std::cin.ignore(std::cin.rdbuf()->in_avail());
  std::cin.get();
#endif
  return;
}
