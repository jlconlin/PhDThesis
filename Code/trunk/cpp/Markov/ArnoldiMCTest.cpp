/*ArnoldiMCTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiMCTest.cpp 547 2009-05-16 01:23:43Z jlconlin $
 $Revision: 547 $
 $Date: 2009-05-15 19:23:43 -0600 (Fri, 15 May 2009) $

 This is used to test the ArnoldiMC class
 */

#include<iostream>
#include<vector>
#include "CartesianMesh1D.hh"
#include "ArnoldiMCTest.h"
#include "ArnoldiMC.h"
#include "LinearSpaceSource.h"

#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

int main( int argc, char* argv[] ){
    cout << "I'm testing ArnoldiMC..." << endl;
    vector<unsigned long> seed(2, 1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << endl;
    
    double width = 20.0;
    unsigned int zones = 1;
    unsigned int bins = 50;
    unsigned int I = 10;
    unsigned int n = 5;     // Desired number of Eigenvectors
    unsigned int active = 20;     // Number of Restarts
    unsigned int inactive = 2;
    unsigned int histories = 5000;

    // Initial Uniform Source
    CartesianMesh<OneD> SourceMesh(width, 1.0, bins);
    HistSource::ublas_vec_t Probabilities(bins, 1.0);
    HistSource InitialSource(seed, SourceMesh, Probabilities);

    // Geometry
    std::vector<double> ZoneWidths(zones, width/zones);
    CartesianMesh<OneD> GeoMesh(ZoneWidths, 1.0);
    ublas::vector<Material> Mats(zones, Material(0.5,0.5,0.0,1.0) );
    Field<Zone<OneD>,Material,ublas::vector<Material> > GeoField(GeoMesh, Mats);

    ArnoldiMCTest AMCTest(seed, GeoField, SourceMesh);
//  AMCTest.CompareRAM(InitialSource, n, I, active, inactive, histories);
//  AMCTest.IRAM(InitialSource, n, I, active, inactive, histories);
//  AMCTest.ERAM(InitialSource, n, I, active, inactive, histories);
//  AMCTest.ValueStats();
//  AMCTest.VectorStats();
//  AMCTest.MeanValues();

    {
    cout << "-----------------------------------------------"
         << "\nDoing Arnoldi's method with LinearSpaceSource"
         << "\n---------------------------------------------\n" << endl;
    // Initial Uniform LinearSpace Source
    ublas::vector<double> Slopes(bins, 0.0);
    ublas::vector<double> Intercepts(bins, 1.0);
    LinearSpaceSource LSS(Slopes, Intercepts, SourceMesh, seed);
    LSS.Normalize();
    cout << "Initial Source: " << LSS << endl;

    ArnoldiMC<LinearSpaceSource> AMC(seed, GeoField, SourceMesh, 0.0, 2);
    AMC.RAM(LSS, n, I, active, inactive, histories, EXPLICIT);

    }
    return 0;
}

