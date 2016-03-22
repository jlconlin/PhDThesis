/*HistSourceTest.cpp
 $Author: jlconlin $
 $Id: HistSourceTest.cpp 514 2009-04-10 18:28:08Z jlconlin $
 $Revision: 514 $
 $Date: 2009-04-10 12:28:08 -0600 (Fri, 10 Apr 2009) $

 This is used to test the HistSource class.
 */

#include <iostream>
#include <algorithm>
#include "boost/numeric/ublas/vector.hpp"
#include "HistSource.h"
#include "gnuplot_i.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "RandomLib/Random.hpp"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;

bool Comp( const Zone<OneD>& Z, double x);
void score(HistSource&, Particle&);
void testSampling();


int main( int argc, char* argv[] ){

    testSampling();
    return 0;
}

void testSampling(){
    vector<unsigned long> seed(1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << "\n";
    
    unsigned int numZones = 10;
    unsigned int N = 100000;

    CartesianMesh<OneD> sourceMesh(1.0, 1.0, numZones);

    //Uniform source
    HistSource::ublas_vec_t Probabilities(numZones, 1.0);
    Probabilities[2] = 2;
    Probabilities[3] = 2;
    Probabilities[4] = 2;
    Probabilities[5] = -2;
    Probabilities[6] = -2;
    Probabilities[7] = -1;
    Field<Zone<OneD>, double, vector<double> > uSource(sourceMesh, 
        Probabilities);

    HistSource hSource(seed, sourceMesh, Probabilities);
    hSource.makeCDF();

    Gnuplot G("lines");
    G.set_title("Testing HistSource Sampling.");
    G.set_xlabel("Bin");
    G.set_ylabel("Sampling Probability");
    G.set_style("histograms fill solid border -1");

    ublas::vector<double> PDF = hSource.PDF();
    PDF /= ublas::norm_2(PDF);
    G.plot_x(PDF, "Original Source");

    HistSource::ublas_vec_t p(numZones, 0.0);

    Particle P(seed, sourceMesh.getZone(0.0) );
    for(int i = 0; i < 3; ++i ){
        HistSource sampledSource(seed, sourceMesh, p);
        for( int j = 0; j < N; ++j ){
            hSource.sample(P);
            score(sampledSource, P);
        }
        PDF = sampledSource.PDF();
        PDF /= ublas::norm_2(PDF);
        G.plot_x(PDF, "Sampled Source");
    }

    sleep(2.0);
}

void score(HistSource& source, Particle& P){
    const Zone<OneD>& Z = source.getZone(P.x());
    source[Z] += P.weight();
}

bool Comp( const Zone<OneD>& Z, double x){
    if (Z.getRightNode().x() < x) return true;
    else return false;
}
