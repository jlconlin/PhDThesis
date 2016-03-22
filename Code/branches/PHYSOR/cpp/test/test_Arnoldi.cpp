/*test_Arnoldi.cpp
 $Author: jlconlin $
 $Id: test_Arnoldi.cpp 397 2008-10-13 21:06:07Z jlconlin $
 $Revision: 397 $
 $Date: 2008-10-13 15:06:07 -0600 (Mon, 13 Oct 2008) $

 This file is used for testing the Arnoldi class by testing its base classes.

 */


#include<cstdlib>
#include<iostream>
#include<complex>
#include "ArnoldiMatrix.h"
#include "ArnoldiMC.h"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
#include "gnuplot_i.hpp"
#include "Utilities.h"

using std::cout;
using std::complex;

int main( int argc, char* argv[]){
    if( argc < 8 ){
      cout << "Program usage:\ttest_Arnoldi D I Noise H R width zones\n"
           << "where D is the dimension of matrix, I is number of Arnoldi\n"
           << "iterations and Noise is the standard deviation of the Noise\n"
           << "and H is the number of histories per Arnoldi Iteration."
           << std::endl;
      return 1;
    }
    int D = std::atoi(argv[1]);
    int I = std::atoi(argv[2]);
    double Noise = std::atof(argv[3]);
    int H = std::atoi(argv[4]);
    int R = std::atoi(argv[5]);
    double width = std::atof(argv[6]);
    int zones = std::atoi(argv[7]);

    vector<unsigned long> seed(2);
//  seed[0] = RandomLib::Random::SeedWord();
    seed[0] = 3910633392;
    seed[1] = 1;
    std::cout << "Master seed: " << seed[0] << "\n";

    // Starting vector
    Teuchos::SerialDenseVector<int, double> q(D);
    for( int i = 0; i < D; ++i ){
        q(i) = 1.0;
    }

#if 0
    {
    // ArnoldiMatrix
    Teuchos::SerialDenseMatrix<int, double> A(D, D);
    // Define matrix elements
    for( int i = 0; i < D; ++i ){
        A(i,i) = i + 1;
    }
    A(2,1) = 1.0;

    ArnoldiMatrix arnM(A, seed, Noise, true);
//    arnM.arnoldi(q, I);
    cout << "\n\nEigenvalues:\n" << arnM.Values() << std::endl;
//  cout << "Eigenvectors:\n" << arnM.Vectors() << "\n";

    // Restarted Arnoldi
    cout << "Restarted Arnoldi\n";
    arnM.ERAM(q, R, I);
    cout << "\n\nEigenvalues:\n" << arnM.Values() << std::endl;
//  cout << "Eigenvectors:\n" << arnM.Vectors() << "\n";

    // Arnoldi Monte Carlo Particle Transport
    cout << "\n\n\n----------------------------------------\n"
         << "Now running Monte Carlo Particle Transport Arnoldi..." << std::endl;`
}
#endif

    int active = 100;
    int inactive = 20;
//  int zones = 1;

    Mesh meshU(width, 1.0, zones);     //Slab geometry, one zone, 20 units wide
    vector<Material> mat(zones, Material(0.5, 0.5, 0.0, 1.0));   //'Standard' material
    Field<Zone, Material> geometry(meshU, mat);

    // Initial uniform HistSourcepower
    vector<double> sourceProbability(D, 1.0);
    Mesh meshS(width, 1.0, D);     // Scoring Mesh
    Field<Zone, double> uSource(meshS, sourceProbability);    
    HistSource uHist(seed, uSource); // Initial uniform Source

    ArnoldiMC ArnMC(seed, geometry, meshS);
    
    // Restarted Arnoldi
    cout << "\n\nRestarted Arnoldi\n";
    ArnMC.ERAM(uHist, 2, R, I, H, ARNOLDI);
    cout << "\n\nEigenvalues:\n" << ArnMC.Values() << std::endl;
    valarray<complex<double> > ComVector = ArnMC.MeanVectors()[0];
    cout << "\n\nDominant Eigenvector:\n" << ComVector << std::endl;

    Gnuplot G("histeps", false);
    G.set_title("Eigenvectors");
    vector<double> RealVector;
    vector<double> ImagVector;
    SplitComplexValarray(ComVector, RealVector, ImagVector);

    G.plot_x(RealVector, "l-0");

    // Plot dominant Eigenvalue
    Gnuplot GValues("errorbars", false);
    vector<double> MeanValue; vector<double> StdDevValue;
    ArnMC.RealMeanValues(0, MeanValue, StdDevValue);

    vector<double> HperR = ArnMC.HperR();
//  vector<double> x( HperR.begin(), HperR.end() );
    vector<double> x(MeanValue.size());
    vector<double>::iterator xIter = x.begin();
    for( int i = 1; xIter != x.end(); ++xIter, ++i) *xIter = i;

    cout << "\nx-values:\n" << x << "\nMean Estimates:\n" << MeanValue
         << "\nStd. Dev.:\n" << StdDevValue << std::endl;

    GValues.set_xlabel("Active Iteration");
    GValues.set_ylabel("Eigenvalue Estimate");
    GValues.plot_xy_err(x, MeanValue, StdDevValue, "l-0");
    Gnuplot GVectors("histeps", false);
    GVectors.set_xlabel("Slab Width (mfp)");
    GVectors.set_ylabel("Mean Eigenvector Estimate");
    vector<double> centers = meshS.ZoneCenters();
    vector<double> MeanRitz; vector<double> StdDevRitz;
    ArnMC.RealMeanVectors(0, MeanRitz, StdDevRitz);
    cout << "Mean Vector size = " << MeanRitz.size() 
         << " vector:\n" << MeanRitz
         << "\nStdDev Vector size = " << StdDevRitz.size() 
         << " vector:\n" << StdDevRitz
         << "\nEdges size = " << centers.size() << " vector:\n" << centers 
         << std::endl;
    GVectors.plot_xy(centers, MeanRitz, "Arnoldi");
    sleep(1);


    
    return 0;
}
