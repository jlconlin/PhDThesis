/*ArnoldiMC<HistSource>Test.h
 $Author: jlconlin $
 $Id: ArnoldiMCTest.h 522 2009-04-24 13:35:29Z jlconlin $
 $Revision: 522 $
 $Date: 2009-04-24 07:35:29 -0600 (Fri, 24 Apr 2009) $

 This class is used to test the ArnoldiMC<HistSource> class.  It is a friend class to 
 ArnoldiMC<HistSource> and therefore has access to all it's attributes.
 */

#ifndef ARNOLDIMCTEST_H
#define ARNOLDIMCTEST_H

#include<iostream>
#include<vector>
#include<complex>
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "gnuplot_i.hpp"
#include "Field.hh"
#include "CartesianMesh1D.hh"
#include "ArnoldiMC.h"
#include "HistSource.h"
#include "Matrices.hpp"
#include "Utilities.h"

#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

class ArnoldiMCTest : public ArnoldiMC<HistSource> {
    public:
        ArnoldiMCTest(const vector<unsigned long>& seed,
                const field_t& geometry,
                CartesianMesh<OneD>& ScoringMesh)
            : ArnoldiMC<HistSource>(seed, geometry, ScoringMesh, 0.0, 2) {  }

        void CompareRAM( HistSource& source, unsigned int n, unsigned I, 
                unsigned int active, unsigned int inactive, 
                unsigned int histories);
        void IRAM( HistSource& source, unsigned int n, unsigned I, 
                unsigned int active, unsigned int inactive, 
                unsigned int histories);
        void ERAM( HistSource& source, unsigned int n, unsigned I, 
                unsigned int active, unsigned int inactive, 
                unsigned int histories);
        void arnoldi(HistSource& v, unsigned int I, unsigned int);

        void ValueStats();
        void VectorStats();
        void MeanValues();

};

void ArnoldiMCTest::arnoldi(HistSource& H, unsigned int I, 
        unsigned int histories){
    cout << "\n============================\n"
         << "Testing ArnoldiMC<HistSource>Test::arnoldi"
         << "\n============================\n" << endl;

    ArnoldiMC<HistSource>::arnoldi(H, I, histories);
}

void ArnoldiMCTest::CompareRAM( HistSource& source, unsigned int n, unsigned I, 
        unsigned int active, unsigned int inactive, unsigned int histories){
    cout << "\n============================\n"
         << "Testing ArnoldiMC<HistSource>Test::CompareRAM"
         << "\n============================\n" << endl;

    ublas::vector<complex<double> > IMean(n);
    ublas::vector<complex<double> > IStdDev(n);
    ublas::vector<complex<double> > EMean(n);
    ublas::vector<complex<double> > EStdDev(n);

    ublas::vector<double> edges = source.Edges();
    ublas::vector<double> centers = source.Centers();

    Gnuplot G = Gnuplot("histeps");
    cout << "\nIMPLICIT" << endl;
    ArnoldiMC<HistSource>::RAM(source, n, I, active, inactive, histories, IMPLICIT);
    MeanVector(0, IMean, IStdDev);  // Get mean vector
    G.plot_xy(centers, real(IMean), "IMPLICIT-0");
    MeanVector(1, IMean, IStdDev);  // Get mean vector
    G.plot_xy(centers, real(IMean), "IMPLICIT-1");
    cout << "\nEXPLICIT" << endl;
    ArnoldiMC<HistSource>::RAM(source, n, I, active, inactive, histories, EXPLICIT);
    MeanVector(0, EMean, EStdDev);  // Get mean vector
    G.plot_xy(centers, real(EMean), "EXPLICIT-0");
    MeanVector(1, EMean, EStdDev);  // Get mean vector
    if( inner_prod(EMean, IMean).real() < 0.0 ) EMean *= -1.0;
    G.plot_xy(centers, real(EMean), "EXPLICIT-1");
    sleep(2);
}

void ArnoldiMCTest::ERAM( HistSource& source, unsigned int n, unsigned I, 
        unsigned int active, unsigned int inactive, unsigned int histories){
    cout << "\n============================\n"
         << "Testing ArnoldiMC<HistSource>Test::ERAM"
         << "\n============================\n" << endl;

    ArnoldiMC<HistSource>::RAM(source, n, I, active, inactive, histories, EXPLICIT);
}

void ArnoldiMCTest::IRAM( HistSource& source, unsigned int n, unsigned I, 
        unsigned int active, unsigned int inactive, unsigned int histories){
    cout << "\n============================\n"
         << "Testing ArnoldiMC<HistSource>Test::IRAM"
         << "\n============================\n" << endl;

    ArnoldiMC<HistSource>::RAM(source, n, I, active, inactive, histories, IMPLICIT);

    // Get mean vector
    ublas::vector<complex<double> > Mean(n);
    ublas::vector<complex<double> > StdDev(n);
    MeanVector(0, Mean, StdDev);
    print_v(Mean, "Mean vector");
    print_v(StdDev, "SD vector");

}

void ArnoldiMCTest::MeanValues(){
    cout << "\n============================\n"
         << "Testing ArnoldiMC<HistSource>Test::MeanValues"
         << "\n============================\n" << endl;

    unsigned int R = 5;
    _n = 5;
    _EigenvaluesEst.resize(_n, R);
    Matrices::Full(_EigenvaluesEst);
    print_m(_EigenvaluesEst, "EigenvaluesEst");
    std::vector<complex<double> > Mean;
    std::vector<complex<double> > SD;
    for( int i = 0; i < R; ++i ){
        cout << "i = " << i << "\n\trow: " << row(_EigenvaluesEst, R-i-1) << endl;
        ArnoldiMC<HistSource>::MeanValues(Mean, SD, i, false);
        cout << "\tMean: " << Mean 
             << "\n\tSD: " << SD << endl;
    }
}

void ArnoldiMCTest::ValueStats(){
    unsigned int R = 10;
    _n = 2;
    _EigenvaluesEst.resize(_n, R);
    _Eigenvalues.resize(_n);    _Eigenvalues *= 0;

    for( int i = 0; i < R; ++i ){
        _R = i;
        _Eigenvalues[1] = complex<double>(i+1,0);
        _ValueStats();
        cout << "Restart #: " << i << ", Eigenvalue  = " << _Eigenvalues[1] 
             << ", Mean = " << _MeanValue
             <<  ", std.dev = " << _StdDevValue << endl;

    }
}

void ArnoldiMCTest::VectorStats(){

    unsigned int n = 5;
    c_mat_t V(n,n);
    Matrices::Full(V);
    print_m(V, "V");

    _length = n;
    
    _RitzvectorsEst.push_back(V);
    V *= -1.0; print_m(V, "V*-1");
    _RitzvectorsEst.push_back(V);
    V *= -2.0; print_m(V, "V*2");
    _RitzvectorsEst.push_back(V);
    V *= 4.0/2.0; print_m(V, "V*4");
    _RitzvectorsEst.push_back(V);

    c_vec_t Mean;
    c_vec_t SD;
    print_v(Mean, "Mean-before");
    
    for( int i = 0; i < n; ++i ){
        cout << "i = " << i << endl;
        MeanVector(i, Mean, SD);
        print_v(Mean, "Mean");
        print_v(SD, "SD");
    }

}


#endif

