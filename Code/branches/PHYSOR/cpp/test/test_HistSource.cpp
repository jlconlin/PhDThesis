/*test_HistSource.cpp
 $Author: jlconlin $
 $Id: test_HistSource.cpp 270 2008-03-05 21:49:39Z jlconlin $
 $Revision: 270 $
 $Date: 2008-03-05 14:49:39 -0700 (Wed, 05 Mar 2008) $

 This file is used for testing the HistSource class.

 */

#include <iostream>
#include <vector>
#include <numeric>
#include "HistSource.h"
#include "gnuplot_i.hpp"
#include "Utilities.h"

using std::cout;
using std::vector;

void score(HistSource&, Particle&);
void Normalize(vector<double>&);

int main(){
    vector<unsigned long> seed(1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << "\n";
    
    unsigned int numZones = 10;
    unsigned int N = 10000;
    
    cout << "Creating Gnuplot instance.\n";
    Gnuplot G("lines", false);

    cout << "Here I create a mesh of " << numZones 
              << " zones each of width 1.0.\n";
    Mesh meshU(1.0, 1.0, numZones);
    
    //Uniform source
    cout << "Here I define/create the source probabilities for "
              << "each Zone.\n";
    vector<double> sourceProbability(numZones, 1.0);
    sourceProbability[2] = 2;
    sourceProbability[3] = 2;
    sourceProbability[4] = 2;
    sourceProbability[5] = -2;
    sourceProbability[6] = -2;
    sourceProbability[7] = -1;
    Field<Zone, double> uSource(meshU, sourceProbability);

    HistSource hSource(seed, uSource);

    cout << "Setting Gnuplot information.\n";
    G.set_title("Testing HistSource Sampling.");
    G.set_xlabel("Bin");
    G.set_ylabel("Sampling Probability");
    G.set_style("histograms fill solid border -1");
    vector<double> PDF = hSource.getPDF();
    Normalize(PDF);
    cout << "PDF:\n" << PDF << std::endl;
    G.plot_x(PDF, "Original Source");


    vector<double> p(numZones, 0.0);
    HistSource sampledSource(seed, meshU, p);

    const Zone* tmpZone = new Zone(1.0, 1.0, 1);
    Particle P(seed, tmpZone);

    for( int i = 0; i < N; ++i ){
        hSource.sample(P);
        score(sampledSource, P);
    }

    PDF = sampledSource.getPDF();
    Normalize(PDF);
    G.plot_x(PDF, "Sampled Source");
    sleep(5);

    return 0;
}


void Normalize(vector<double>& PDF){

    double s = 0;
    vector<double>::iterator PDFIter = PDF.begin();
    for( ; PDFIter != PDF.end(); ++PDFIter ) s += std::abs(*PDFIter);
    cout << "s = " << s << "\nPDF:\n" << PDF << std::endl;

    PDFIter = PDF.begin();
    for( ; PDFIter != PDF.end(); ++PDFIter ) *PDFIter = *PDFIter/s;
    PDFIter = PDF.begin();
    for( ; PDFIter != PDF.end(); ++PDFIter ) s += std::abs(*PDFIter);
    cout << "s = " << s << "\nPDF:\n" << PDF << std::endl;
}

void score(HistSource& source, Particle& P){
    const Zone* Z = source.getZone(P.x());
    source[*Z] += P.weight();
}
