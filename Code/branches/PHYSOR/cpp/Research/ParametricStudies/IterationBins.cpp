/*IterationBins.cpp
 $Author: jlconlin $
 $Id: IterationBins.cpp 295 2008-05-05 17:23:46Z jlconlin $
 $Revision: 295 $
 $Date: 2008-05-05 11:23:46 -0600 (Mon, 05 May 2008) $

 This file explores the relationship between bins and Iterations.

 */

#include<iostream>
#include<sstream>
#include "ArnoldiMC.h"
#include "Material.h"
#include "Field.hh"
#include "OneDCartMesh.hh"
#include "HistSource.h"
#include "gnuplot_i.hpp"

using std::cout;
using std::endl;
using std::setprecision;
using std::scientific;
using std::fixed;

int main(){
    cout << "I'm studing IterationBins." << endl;
    unsigned int inactive = 2;
    unsigned int active  = 100;
    unsigned int zones = 1;
    unsigned int histories = 5000;
    double width = 100;
    double relaxed = 0.0;
    bool verbose = false;
    vector<unsigned long> seed(2, 1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << endl;

    vector<unsigned int> Iterations;
    Iterations.push_back(5);
    Iterations.push_back(7);
    Iterations.push_back(10);
    Iterations.push_back(15);
    Iterations.push_back(20);
    vector<double> IterationsD( Iterations.begin(), Iterations.end() );
    vector<unsigned int> Bins;
    Bins.push_back(25);
    Bins.push_back(50);
    Bins.push_back(75);
    Bins.push_back(100);

    // Geometry
    Mesh geoMesh(width, 1.0, zones);
    vector<Material> Mat(zones, Material(0.5,0.5,0.0,1.0) );
    Field<Zone, Material> Geometry(geoMesh, Mat);

    // Prepare plotters
    Gnuplot gnuReal("errorlines", false);
    gnuReal.set_title("Real Part");
    gnuReal.set_xlabel("Krylov Subspace Size");
    gnuReal.set_ylabel("Ritz Value Estimate");
    Gnuplot gnuImag("errorlines", false);
    gnuImag.set_title("Imaginary Part");
    gnuImag.set_xlabel("Krylov Subspace Size");
    gnuImag.set_ylabel("Ritz Value Estimate");

    vector<unsigned int>::iterator BinIter = Bins.begin();
    for( ; BinIter != Bins.end(); ++BinIter ){
        cout << "\n---------------------------------------\n"
             << "Bins = " << *BinIter << endl;
        // Initial Source
        Mesh DiscMesh(width, 1.0, *BinIter);    // Discretization Mesh
        vector<double> SourceProbability(*BinIter, 1.0);
        Field<Zone, double> SourceField(DiscMesh, SourceProbability);
        HistSource HSource(seed, SourceField);

        vector<double> L1Real, L1Imag;  // Real and imaginary parts of dominant
        vector<double> L1RealSD, L1ImagSD;  // Standard Devations
        vector<double> L2Real, L2Imag;  // Real and imaginary parts of second
        vector<double> L2RealSD, L2ImagSD;  // Standard Devations
        // Iterate over all Iteration values
        vector<unsigned int>::iterator ITIter = Iterations.begin();
        for( ; ITIter != Iterations.end(); ++ITIter ){
            ArnoldiMC AMC(seed, Geometry, DiscMesh, relaxed, verbose);
            AMC.ERAM(HSource, inactive, active, *ITIter, histories);

//          L1Real.push_back(AMC.MeanValues(0).back().real());
//          L1Imag.push_back(AMC.MeanValues(0).back().imag());
//          L1RealSD.push_back(AMC.StdDevValues(0).back().real());
//          L1ImagSD.push_back(AMC.StdDevValues(0).back().imag());
//          L2Real.push_back(AMC.MeanValues(1).back().real());
//          L2Imag.push_back(AMC.MeanValues(1).back().imag());
//          L2RealSD.push_back(AMC.StdDevValues(1).back().real());
//          L2ImagSD.push_back(AMC.StdDevValues(1).back().imag());

            cout << "Iterations: " << *ITIter << setprecision(7) << fixed
                 << ", Value = " << L1Real.back() << " " 
                 << scientific << setprecision(3) << L1Imag.back() << "j" 
                 << fixed << setprecision(7) << ", StdDev = " << L1RealSD.back() 
                 << " " << scientific << setprecision(3) << L1ImagSD.back() 
                 << "j" << endl;
            cout << "Iterations: " << *ITIter << setprecision(7) << fixed
                 << ", Value = " << L2Real.back() << ", " 
                 << scientific << setprecision(3) << L2Imag.back() << "j" 
                 << fixed << setprecision(7) << ", StdDev = " << L2RealSD.back() 
                 << " " << scientific << setprecision(3) << L2ImagSD.back() 
                 << "j" << endl;
        }

        ostringstream L1Title, L2Title;
        L1Title << "L1-" << *BinIter;
        L2Title << "L2-" << *BinIter;
        gnuReal.plot_xy_err( IterationsD, L1Real, L1RealSD, L1Title.str() );
        gnuReal.plot_xy_err( IterationsD, L2Real, L2RealSD, L2Title.str() );
        gnuImag.plot_xy_err( IterationsD, L1Imag, L1ImagSD, L1Title.str() );
        gnuImag.plot_xy_err( IterationsD, L2Imag, L2ImagSD, L2Title.str() );
        sleep(1);
    }


    return 0;
}
