/*ArnoldiMatrixTest.h
 $Author: jlconlin $
 $Id: ArnoldiMatrixTest.h 342 2008-08-04 19:55:07Z jlconlin $
 $Revision: 342 $
 $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

 This class is used to test the ArnoldiMatrix class.  It is a friend class to 
 Arnoldi and therefore has access to all it's attributes.
 */

#ifndef ARNOLDIMATRIXTEST_H
#define ARNOLDIMATRIXTEST_H

#include<iostream>
#include<limits>
#include "ArnoldiMatrix.h"
#include "boost/numeric/ublas/io.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
class ArnoldiMatrixTest : public ArnoldiMatrix {
    public:
        ArnoldiMatrixTest(ArnoldiMatrix::Mat_t& A, bool v = true,
                double Tol = std::numeric_limits<double>::epsilon() )
            : ArnoldiMatrix(A, true)
        {   }

        const ArnoldiMatrix::Mat_t& Matrix () const 
            { return _A; }

        void IRAM();
        void arnoldi();
        void Av();
    private:

};

void ArnoldiMatrixTest::IRAM(){
    cout << "====================================\n"
         << "Testing IRAM." << endl;
    
    unsigned int n = 2;
    unsigned int m = 4;
    unsigned int R = 5;
    cout << "n = " << n << ", m = " << m << ", R = " << R << endl;

    ArnoldiMatrix::dVec_t q(_length);
    for( int i = 0; i < q.size(); ++i ) q(i) = 1.0;
    print_v(q, "q");
    ArnoldiMatrix::IRAM(q, n, m, R);
    print_v(_Eigenvalues, "values");
    print_m(_Ritzvectors, "vectors");
    cout << "\nAx - lx: " << endl;
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
    Arnoldi::dVec_t q(_length);
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

    Arnoldi::cVec_t q(_length);

    for( int i = 0; i < q.size(); ++i ) q(i) = i+1;

    q = q/ublas::norm_2(q); // Normalize q
    // Set first column of _V to be starting vector
    cColumn_t(_V, 0) = q;
    print_m(_A, "_A");
    print_m(_V, "_V");
    _k = 0;
    q = _Av();
    print_v(q, "result");

}

#endif

