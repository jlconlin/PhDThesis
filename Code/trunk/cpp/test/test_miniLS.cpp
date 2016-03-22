/*test_miniLS.cpp

    This file is used to test a very simple Linear Space source to figure out 
    how I should implement it.

 */

#include<iostream>
#include<vector>
#include "gnuplot_i.hpp"
#include "miniLS.hpp"
#include "boost/tr1/random.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "RandomLib/Random.hpp"
#include "CartesianMesh1D.hh"
#include "Mesh_AddOns.hh"
#include "Field.hh"
#include "Field_AddOns.hh"

using std::cout;
using std::endl;
using namespace std::tr1;

namespace ublas = boost::numeric::ublas;

std::vector<double> CfromMB(double, double, double, double);
void PlotxS(std::vector<double>& xS, double width, Gnuplot& G, std::string Name,
        unsigned int N, unsigned int bins);

void OneBin(unsigned int);

int main(int argc, char* argv[] ){
    cout << "Testing/learning LinearS(s) " << endl;

    unsigned int N = std::atoi(argv[1]);

    OneBin(N);

    return 0;
}

void OneBin( unsigned int N ){
    cout << "\nOneBin..." << endl;
    std::vector<unsigned long> seed(1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master Seed: " << seed[0] << endl;

    Gnuplot G("lines");

    double xmin(0.0);
    double xmax(5.0);
    double slope(1.0);
    double intercept(2.0);

    std::vector<double> Coefficients(2, 0.0);
    Coefficients = CfromMB(slope, intercept, xmin, xmax);

    CartesianMesh<OneD> GeometryMesh(xmax, 1.0, 1); 

    LinearS<2> LS(Coefficients, xmin, xmax);

    cout << LS << endl;

    LinearSS<2> LSS(seed, GeometryMesh, Coefficients );

    cout << "\nLinearSS:\n" << LSS << endl;

}

std::vector<double> CfromMB(double m, double b, double xmin, double xmax){
    std::vector<double> Coefficients(2, 0.0);
    Coefficients[0] = b + (xmax + xmin)/2.0;
    Coefficients[1] = m*(xmax - xmin)/2.0;

    return Coefficients;
}

void PlotxS(std::vector<double>& xS, double width, Gnuplot& G, std::string Name,
        unsigned int N, unsigned int bins){

    CartesianMesh<OneD> M(width, 1.0, bins);
    Field<Zone<OneD>, double, ublas::vector<double> > field(M, 0.0);
    ublas::vector<double> centers = ZoneCenters(M);
    ublas::vector<double> binValues;
    double binWidth = width/bins;
    G.set_style("histeps");

    // Put samples into bins
    std::vector<double>::iterator xIter = xS.begin();
    for( ; xIter != xS.end(); ++xIter ) field[field.getZone(*xIter)] += 1.0;    

    binValues = GetFieldContainer(field);
    binValues /= N*binWidth;

    G.plot_xy( centers, binValues, Name );
}


