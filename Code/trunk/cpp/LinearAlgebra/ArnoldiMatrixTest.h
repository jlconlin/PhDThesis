/*ArnoldiMatrixTest.h
 $Author: jlconlin $
 $Id: ArnoldiMatrixTest.h 528 2009-04-29 20:14:39Z jlconlin $
 $Revision: 528 $
 $Date: 2009-04-29 14:14:39 -0600 (Wed, 29 Apr 2009) $

 This class is used to test the ArnoldiMatrix class.  It is a friend class to 
 Arnoldi and therefore has access to all it's attributes.
 */

#ifndef ARNOLDIMATRIXTEST_H
#define ARNOLDIMATRIXTEST_H

#include<iostream>
#include<limits>
#include<vector>
#include "ArnoldiMatrix.h"
#include "boost/numeric/ublas/io.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
class ArnoldiMatrixTest : public ArnoldiMatrix {
    public:
        ArnoldiMatrixTest(ArnoldiMatrix::Mat_t& A, 
                std::vector<unsigned long>& seed, bool v = true,
                double Tol = std::numeric_limits<double>::epsilon() )
            : ArnoldiMatrix(A, seed, true)
        {   }

        const ArnoldiMatrix::Mat_t& Matrix () const 
            { return _A; }

        void RAM(unsigned int, unsigned int, unsigned int, RestartMethod, 
                double);
        void arnoldi();
        void Av();
    private:

};

void ArnoldiMatrixTest::RAM(unsigned int n, unsigned int I, unsigned int R, 
        RestartMethod M, double){
    cout << "====================================\n"
         << "Testing RAM." << endl;
    
    ArnoldiMatrix::T_vec_t q(_length, 1.0);
    print_v(q, "Starting vector");

    ArnoldiMatrix::RAM(q, n, I, R, M);

    print_v(_Eigenvalues, "values");
    print_m(_Ritzvectors, "vectors");

    cout << "\nThe following values should be zero if everthing went well"
         << "\nAx - lx: " << endl;
    for( int i = 0; i < _Ritzvectors.size2(); ++i ){
        ublas::vector<complex<double> > tmp(_length);
        tmp = ublas::prod( _A, column(_Ritzvectors, i) );
        cout << tmp - _Eigenvalues(i)*column(_Ritzvectors,i) << endl;
    }

}

void ArnoldiMatrixTest::arnoldi(){
    cout << "====================================\n"
         << "Running arnoldi(q, I)." << endl;
    _length = _A.size2();
    unsigned int I = 5;
    T_vec_t q(_length);
    for( int i = 0; i < q.size(); ++i ) q(i) = 1.0;
    print_v(q, "q");
    cout << "I = " << I << endl;

    ArnoldiMatrix::arnoldi(q, I-1);
    _MakeRitzPairs();
    print_m(_H, "H");
    print_v(_Eigenvalues, "values");
    print_m(_Ritzvectors, "vectors");
    cout << "\nAx - lx: " << endl;
    for( int i = 0; i < _Ritzvectors.size2(); ++i ){
        ublas::vector<complex<double> > tmp(_length);
        tmp = ublas::prod( _A, column(_Ritzvectors, i) );
        cout << tmp - _Eigenvalues(i)*column(_Ritzvectors,i) << endl;
    }
}

void ArnoldiMatrixTest::Av(){
    cout << "====================================\n"
         << "Checking Matrix-vector product." << endl;
    _length = _A.size2();
    _V.resize( _length, 1 );

    T_vec_t q(_length);

    for( int i = 0; i < q.size(); ++i ) q(i) = i+1;

    q = q/ublas::norm_2(q); // Normalize q
    // Set first column of _V to be starting vector
    T_column_t(_V, 0) = q;
    print_m(_A, "_A");
    print_m(_V, "_V");
    _k = 0;
    q = _Av();
    print_v(q, "result");

}

#endif

