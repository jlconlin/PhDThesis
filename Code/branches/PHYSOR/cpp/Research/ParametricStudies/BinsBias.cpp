/*BinsBias.cpp
 $Author: jlconlin $
 $Id: BinsBias.cpp 299 2008-05-15 00:36:42Z jlconlin $
 $Revision: 299 $
 $Date: 2008-05-14 18:36:42 -0600 (Wed, 14 May 2008) $

 This file is used to determine the effect of the discretization error and 
 whether it goes away with more particles.

 */

#include<iostream>
#include<sstream>
#include<cmath>
#include "ArnoldiMC.h"
#include "Material.h"
#include "Field.hh"
#include "OneDCartMesh.hh"
#include "HistSource.h"
#include "gnuplot_i.hpp"
#include "gnuFile.h"

using std::cout;
using std::endl;
using std::setprecision;
using std::scientific;
using std::fixed;

int main(){
    cout << "I'm studying BinsBias." << endl;
    unsigned int iterations = 15;
    unsigned int inactive = 2;
    unsigned int active  = 20;
    unsigned int zones = 1;
    double width = 20;
    double relaxed = 0.0;
    bool verbose = false;
    vector<unsigned long> seed(2, 1);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << endl;

    double answer = 0.985928;    // Benchmark answer

    vector<unsigned int> bins;
    bins.push_back(30);
    bins.push_back(40);
    bins.push_back(55);
    bins.push_back(70);
    bins.push_back(80);
    bins.push_back(100);
    bins.push_back(120);
    bins.push_back(140);
    bins.push_back(160);
    vector<double> BinsD( bins.begin(), bins.end() );
    vector<unsigned int> Histories;
//  Histories.push_back(1000);
//  Histories.push_back(5000);
//  Histories.push_back(10000);
//  Histories.push_back(50000);
//  Histories.push_back(100000);
//  Histories.push_back(500000);
    Histories.push_back(1000000);
//  Histories.push_back(5000000);

    // Geometry
    Mesh geoMesh(width, 1.0, zones);
    vector<Material> Mat(zones, Material(0.5,0.5,0.0,1.0) );
    Field<Zone, Material> Geometry(geoMesh, Mat);


    Gnuplot gnuReal("errorlines", false);
    gnuReal.set_title("");
    gnuReal.set_xlabel("Discretization Bins");
    gnuReal.set_ylabel("Eigenvalue Bias");
    gnuReal.cmd("set xrange[30:105]");

    gnuFile Outfile("BB-Out.dat");

    // Iterate over all Histories values
    vector<unsigned int>::iterator HistIter = Histories.begin();
    for( ; HistIter != Histories.end(); ++HistIter ){
        cout << "\n---------------------------------------\n"
             << "Histories = " << *HistIter << endl;

        double Bias;
        double BiasSD;
        vector<double> vecBias;
        vector<double> vecBiasSD;
        vector<double> BinWidths;

        // Iterate over bins
        vector<unsigned int>::iterator BinIter = bins.begin();
        for( ; BinIter != bins.end(); ++BinIter ){
            // Initial Source
            Mesh DiscMesh(width, 1.0, *BinIter);    // Discretization Mesh
            vector<double> edges = DiscMesh.ZoneCenters();
            vector<double> SourceProbability(*BinIter, 1.0);
            Field<Zone, double> SourceField(DiscMesh, SourceProbability);
            HistSource HSource(seed, SourceField);
            BinWidths.push_back( width/(*BinIter) );

            ArnoldiMC AMC(seed, Geometry, DiscMesh, relaxed, verbose);
            AMC.ERAM(HSource, inactive, active, iterations, *HistIter);


            Bias = std::abs( answer - AMC.MeanValues(0).back() );
            BiasSD = AMC.StdDevValues(0).back();
            vecBias.push_back(Bias);
            vecBiasSD.push_back(BiasSD);
            cout << "Bins: " << *BinIter << setprecision(7)
                 << ", Value: " << AMC.MeanValues(0).back()
                 << ", Bias: " << Bias << " +/- " << BiasSD << endl;
        }

        ostringstream title;
        title << *HistIter;
        gnuReal.plot_xy_err( BinsD, vecBias, vecBiasSD, title.str() );
        Outfile.append(title.str(), BinsD, vecBias, vecBiasSD);
        Outfile.append(title.str() + "-widths", BinWidths, vecBias, vecBiasSD );
        cout << "\nHistories: " << *HistIter
             << "\nBins: " << BinsD
             << "\nBias: " << vecBias
             << "\nBiasSD: " << vecBiasSD << endl;
        sleep(1);
    }

    return 0;
}


