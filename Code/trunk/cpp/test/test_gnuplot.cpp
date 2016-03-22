/*test_gnuplot.cpp

 */

#include<iostream>
#include<vector>
#include "boost/numeric/ublas/vector.hpp"
#include "gnuplot_i.hpp"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]){

    int n = 5;

    std::vector<double> cX(n,0.0);
    ublas::vector<double> uX(n,0.0);
    cX[0] = 1.0;   uX[0] =-1.0;
    cX[1] = 2.0;   uX[1] =-2.0;
    cX[2] = 3.0;   uX[2] =-3.0;
    cX[3] = 4.0;   uX[3] =-4.0;
    cX[4] = 5.0;   uX[4] =-5.0;

    std::vector<double> cVector(n,0.0);
    ublas::vector<double> uVector(n, 0.0);
    cVector[0] = 1.0;   uVector[0] =-1.0;
    cVector[1] = 2.0;   uVector[1] =-2.0;
    cVector[2] = 3.0;   uVector[2] =-3.0;
    cVector[3] = 2.0;   uVector[3] =-2.0;
    cVector[4] = 1.0;   uVector[4] =-1.0;

    Gnuplot G("lines");
    G.plot_x(cX, "cX");
    G.plot_x(uX, "uX");
    G.plot_xy(cX, cVector, "cX-cVector");
    G.plot_xy(uX, uVector, "uX-uVector");
    sleep(1);
    return 0;
}
