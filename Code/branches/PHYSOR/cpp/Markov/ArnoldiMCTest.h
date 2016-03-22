/*ArnoldiMCTest.h
 $Author: jlconlin $
 $Id: ArnoldiMCTest.h 284 2008-04-09 13:33:27Z jlconlin $
 $Revision: 284 $
 $Date: 2008-04-09 07:33:27 -0600 (Wed, 09 Apr 2008) $

 This class is used to test the ArnoldiMC class.  It is a friend class to 
 ArnoldiMC and therefore has access to all it's attributes.
 */

#ifndef ARNOLDIMCTEST_H
#define ARNOLDIMCTEST_H

#include<iostream>
#include<vector>
#include<complex>
#include "ArnoldiMC.h"
#include "HistSource.h"
#include "Teuchos_SerialDenseVector.hpp"
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;	

using std::cout;
using std::endl;
using std::vector;
using std::complex;

class ArnoldiMCTest : public ArnoldiMC {
    public:
        ArnoldiMCTest(const vector<unsigned long>& seed,
                const Field<Zone, Material>& geometry,
                Mesh& ScoringMesh)
            : ArnoldiMC(seed, geometry, ScoringMesh) {  }

        void ValueStats();
        void VectorStats();
        void Scale();

    private:

};


void ArnoldiMCTest::ValueStats(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing the mean/std.dev calculation for Ritzvalues.\n"
         << "\n-----------------------------------------" << endl;

    int i = 1;
    _Ritzvalues = vector<complex<double> >(3, 0.0);
    _CycleValues = vector<vector<complex<double> > >( _Ritzvalues.size() );
    _MeanValues = vector<vector<complex<double> > >( _Ritzvalues.size() );
    _StdDevValues = vector<vector<complex<double> > >( _Ritzvalues.size() );
    vector<complex<double> >::iterator RVIter;
    for( int j = 0; j < 3; ++j ){
        // Assign values to _Ritzvalues
        cout << "\nj: " << j;
        for( RVIter = _Ritzvalues.begin(); RVIter != _Ritzvalues.end(); 
                ++RVIter, ++i){
            *RVIter = i;
        }
         cout << "_Ritzvalues: " << _Ritzvalues;
        _ValueStats();
    }

    cout << "\n\nCalculated stats.\n"
         << "Cycle values: " << _CycleValues
         << "\nMean Values: " << _MeanValues 
         << "\nStd Dev: " << _StdDevValues << std::endl;
}


void ArnoldiMCTest::VectorStats(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing the mean/std.dev calculation for Ritzvectors.\n"
         << "\n-----------------------------------------" << endl;

    int i = 0;
    _CycleVectors = vector<vector<valarray<complex<double> > > >();
    int n = 3;

    for( int j = 0; j < 3; ++j ){
        _Ritzvectors = vector<valarray<complex<double> > >();
        for( int k = 0; k < 4; ++k ){
            valarray<complex<double> > rv(n);
            for ( int l = 0; l < n; ++l ){
                rv[l] = ++i*(l+1);
            }
            rv /= rv.sum();
            _Ritzvectors.push_back(rv);
        }
//        cout << "j: " << j
//             << ", Ritzvectors:\n" << _Ritzvectors << std::endl;
        _CycleVectors.push_back(_Ritzvectors);
        
    }
    cout << "Cycle vectors:\n";
    for( int k = 0; k < _CycleVectors.size(); ++k ){
        cout << "[" << _CycleVectors[k] << "]\n"
        ;
    }
    _VectorStats();
    cout << "\n\nMean:\n" << _MeanVectors
        << "\nStd Dev:\n" << _StdDevVectors << std::endl;
}


void ArnoldiMCTest::Scale(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing the Scaling funcationality\n"
         << "\n-----------------------------------------" << endl;

    // Preliminary data
    vector<unsigned long> seed(2,1); int bins = 5;
    Mesh DiscMesh(1, 1.0, bins);    // Discretization Mesh
    vector<double> NProb(bins, 0.0);
    for( int i = 0; i < bins; ++i ){
        NProb[i] = i+1;
    }
    Field<Zone, double> NField(DiscMesh, NProb);
    vector<double> CProb(bins, 0.0); CProb[4] = 1.0;
    Field<Zone, double> CField(DiscMesh, CProb);

    _RelaxHistories = 5;

    HistSource N(seed, NField);
    HistSource C(seed, CField);

    cout << "N pre scaling:\n" << N.getPDF()
         << "\nC pre scaling:\n" << C.getPDF() << endl;

    cout << "\nC.size = " << C.size() << endl;

    cSDV postScale;
    postScale = _Scale(N, C);
    cout << "N post scaling:\n" << postScale;

}

#endif

