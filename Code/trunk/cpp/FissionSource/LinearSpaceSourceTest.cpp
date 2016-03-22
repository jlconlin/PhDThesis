/*LinearSpaceSourceTest.cpp

 This is used to test the LinearSpaceSource class.

*/

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "LinearSpaceSource.h"
#include "gnuplot_i.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Field_AddOns.hh"
#include "RandomLib/Random.hpp"
#include "Mesh_AddOns.hh"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;

std::ostream& operator<< (std::ostream&, const std::vector<double>&);
void testSampling(unsigned int);
void OneBin(unsigned int);
void OneBin( unsigned int, double, double, CartesianMesh<OneD>&, 
        std::vector<unsigned long>&, Gnuplot&, std::string );
void UniformPositive( unsigned int, CartesianMesh<OneD>&, 
		std::vector<unsigned long>&, Gnuplot& );
void UniformNegative( unsigned int, CartesianMesh<OneD>&, 
		std::vector<unsigned long>&, Gnuplot& );
void SlopedPositive( unsigned int, CartesianMesh<OneD>&, 
		std::vector<unsigned long>&, Gnuplot& );
void SlopedNegative( unsigned int, CartesianMesh<OneD>&, 
		std::vector<unsigned long>&, Gnuplot& );
void Straddled( unsigned int, CartesianMesh<OneD>&, 
		std::vector<unsigned long>&, Gnuplot& );

void SampleAndScore(unsigned int, LinearSpaceSource&, CartesianMesh<OneD>&, 
        std::vector<unsigned long>&, Gnuplot&, std::string);
void PlotxS(std::vector<double>&, double, Gnuplot&, std::string, unsigned int N,
        unsigned int bins=100);

int main (int argc, char* argv[] ){

    cout << "\nTesting LinearSpaceSource" << endl;
    unsigned int N;
    if( argc < 1 ) N = 10000;
    else N = atoi(argv[1]);

    double xmin(0.0);
    double xmax(5.0);
    double slope(1.0);
    double intercept(0.0);
    LinearSpace LS(slope, intercept, xmin, xmax);
    cout << "LinearSpace:\n " << LS << endl;
    LS.Normalize();
    cout << "LinearSpaceNormalized:\n " << LS << endl;


//  OneBin( N );
    testSampling( N );
    return 0;
}

void testSampling( unsigned int N ) {
    cout << "==================================\n"
         << "testSampling LinearSpaceSourceTest\n"
         << "==================================" << endl;

    vector<unsigned long> seed(1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << "\n";

    // Initial Source
    unsigned int numZones = 10;
    std::vector<double> widths(numZones);
    ublas::vector<double> slopes(numZones);
    ublas::vector<double> intercepts(numZones);
    ublas::vector<double> Coefficients(2*numZones);

    widths[0] = 3.0;    slopes[0] = 2.0/3.0;        intercepts[0] = 1.0;
    widths[1] = 2.0;    slopes[1] = -1.0;           intercepts[1] = 6.0;
    widths[2] = 2.0;    slopes[2] = 0.0;            intercepts[2] = 1.0;
    widths[3] = 5.0;    slopes[3] = -1.0/2.0;       intercepts[3] = 4.5;
    widths[4] = 3.0;    slopes[4] = 0.0;            intercepts[4] = -1.5;
    widths[5] = 3.0;    slopes[5] = 4.0/3.0;        intercepts[5] = -21.5;
    widths[6] = 2.0;    slopes[6] = -1.0/2.0;       intercepts[6] = 11.5;
    widths[7] = 3.0;    slopes[7] = -1.0/5.0;       intercepts[7] = 5.5;
    widths[8] = 3.0;    slopes[8] = 0.0;            intercepts[8] = 0.9;
    widths[9] = 4.0;    slopes[9] = 1.0/5.0;        intercepts[9] = -4.3;


    Coefficients[0] = 2.0/3.0;         Coefficients[1] = 1.0;
    Coefficients[2] = -1.0;            Coefficients[3] = 6.0;
    Coefficients[4] = 0.0;             Coefficients[5] = 1.0;
    Coefficients[6] = -1.0/2.0;        Coefficients[7] = 4.5;
    Coefficients[8] = 0.0;             Coefficients[9] = -1.5;
    Coefficients[10] = 4.0/3.0;        Coefficients[11] = -21.5;
    Coefficients[12] = -1.0/2.0;       Coefficients[13] = 11.5;
    Coefficients[14] = -1.0/5.0;       Coefficients[15] = 5.5;
    Coefficients[16] = 0.0;            Coefficients[17] = 0.9;
    Coefficients[18] = 1.0/5.0;        Coefficients[19] = -4.3;

    CartesianMesh<OneD> sourceMesh(widths);

    ublas::vector<double> edges = 
            ublas::subrange( NodePositions(sourceMesh), 0, numZones );
    ublas::vector<double> centers = ZoneCenters(sourceMesh);

    cout << "\nslopes: " << slopes << "\nintercepts: " << intercepts
         << "\nCoefficients: " << Coefficients << endl;

//  LinearSpaceSource LSS(slopes, intercepts, sourceMesh, seed);
//  cout << "\nLSS:\n" << LSS << endl;
    LinearSpaceSource LSS(seed, sourceMesh, Coefficients);
    cout << "\nLSS:\n" << LSS << endl;

    LSS.Normalize();
//  cout << "\nNormalized:\n" << LSS << "\npdf: " << LSS.PDF()
//       << "\ncdf: " << LSS.CDF() << endl;

    Gnuplot GPlots("lines");
    GPlots << "set xzeroaxis";
    SampleAndScore(N, LSS, sourceMesh, seed, GPlots, "Big");
}

void OneBin( unsigned int N ){
    cout << "==================================\n"
         << "OneBin LinearSpaceSourceTest\n"
         << "==================================" << endl;
    std::vector<unsigned long> seed(1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << "\n";

    // Initial Source
    double width = 5.0;
    CartesianMesh<OneD> sourceMesh(width, 1.0, 1);

    std::ostringstream title; title << N;

    Gnuplot G("lines");
    G.set_title(title.str() );
    G << "set xzeroaxis";

//  UniformPositive(N, sourceMesh, seed, G);
//  UniformNegative(N, sourceMesh, seed, G);
//  SlopedPositive(N, sourceMesh, seed, G);
//  SlopedNegative(N, sourceMesh, seed, G);
    Straddled(N, sourceMesh, seed, G);
}


void OneBin( unsigned int N, double slope, double intercept, 
        CartesianMesh<OneD>& M, std::vector<unsigned long>& seed, Gnuplot& G,
        std::string Name){

    ublas::vector<double> Slopes(1,slope);
    ublas::vector<double> Intercepts(1,intercept);
    LinearSpaceSource LSS(Slopes, Intercepts, M, seed);
    LSS.Normalize();
    
    cout << LSS << endl;

    SampleAndScore(N, LSS, M, seed, G, Name);
}

void SlopedPositive( unsigned int N, CartesianMesh<OneD>& M, 
		std::vector<unsigned long>& seed, Gnuplot& G ){

    cout << "\nSampling on a sloped postive source\n" << endl;

    double slope(1.0);
    double intercept(0.0);

    OneBin(N, slope, intercept, M, seed, G, "PosSlope");
}

void UniformPositive( unsigned int N, CartesianMesh<OneD>& M, 
		std::vector<unsigned long>& seed, Gnuplot& G ){

    cout << "\nSampling Uniformly on a postive source\n" << endl;

    double slope(0.0000);
    double intercept(1.0);

    OneBin(N, slope, intercept, M, seed, G, "UPos");

}

void UniformNegative( unsigned int N, CartesianMesh<OneD>& M,
        std::vector<unsigned long>& seed, Gnuplot& G ){

    cout << "\nSampling Uniformly on a negative source\n" << endl;

    double slope(0.0);
    double intercept(-1.0);

    OneBin(N, slope, intercept, M, seed, G, "UNeg");

}

void SlopedNegative( unsigned int N, CartesianMesh<OneD>& M, 
		std::vector<unsigned long>& seed, Gnuplot& G ){

    cout << "\nSampling on a sloped negative source\n" << endl;

    double slope(0.5);
    double intercept(-7.0);

    OneBin(N, slope, intercept, M, seed, G, "SNeg");
}

void Straddled( unsigned int N, CartesianMesh<OneD>& M, 
		std::vector<unsigned long>& seed, Gnuplot& G ){

    cout << "\nSampling on a straddled source. \n" << endl;

    double slope(-1.5);
    double intercept(5.0);

    OneBin(N, slope, intercept, M, seed, G, "Straddled");

}

void SampleAndScore(unsigned int N, LinearSpaceSource& LSS, 
        CartesianMesh<OneD>& M, std::vector<unsigned long>& seed, Gnuplot& G, 
        std::string name){

    unsigned int size = M.numZones();
    ublas::vector<double> Slopes(size, 0.0);
    ublas::vector<double> Intercepts(size, 0.0);

    // Plot Original source
    ublas::vector<double> x;
    ublas::vector<double> y;
    ublas::vector<double> yStD;
    LSS.PlotPoints(x,y);

    G.set_style("lines");
    G.plot_xy(x, y, name);

//  std::vector<double> xS(N, 0.0);
//  std::vector<double>::iterator xIter;
    Particle P( seed, M.getZone(0.0) );
    for( int i = 0; i < 1; ++i ){
        LinearSpaceSource LSScored(Slopes, Intercepts, M, seed);
        std::ostringstream title; title << name << "-" << (i+1);
        for( int j = 0; j < N; ++j ){
//      for( xIter = xS.begin(); xIter != xS.end(); ++xIter ){
            LSS.sample(P);
            LSScored.score(P, P.weight());
//          *xIter = P.x();
        }

        LSScored.PostScoring(N);
        LSScored.Normalize();
        LSScored.PlotPoints(x,y);
        cout << "Scored source: " << LSScored << endl;

//      LSS.Samples();

        G.set_style("lines");
        G.plot_xy(x,y, title.str()+"s");

        std::ostringstream xTitle; xTitle << "xS-" << (i+1);
//      PlotxS( xS, M.length(), G, xTitle.str(), N, 100 );
    }
    sleep(1.0);
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
    sleep(1.0);
}

std::ostream& operator<< (std::ostream& os, const std::vector<double>& v){
    
    std::vector<double>::const_iterator vecIter = v.begin();
    for( ; vecIter != v.end(); ++vecIter ){
        os << *vecIter << ", ";
    }
    return os;
}

