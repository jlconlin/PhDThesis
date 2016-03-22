/*ArnoldiTest.h
 $Author: jlconlin $
 $Id: ArnoldiTest.h 342 2008-08-04 19:55:07Z jlconlin $
 $Revision: 342 $
 $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

 This class is used to test the Arnoldi class.  It is a friend class to Arnoldi
 and therefore has access to all it's attributes.
 */

#ifndef ARNOLDITEST_H
#define ARNOLDITEST_H

#include<iostream>
#include<complex>
#include<algorithm>
#include<limits>
#include "Utilities.h"
#include "Matrices.hpp"
#include "Arnoldi.h"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/bindings/lapack/hseqr.hpp"
#include "boost/numeric/bindings/lapack/trevc.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::endl;
using std::cout;
using std::vector;
using std::complex;

namespace ublas =  boost::numeric::ublas;
namespace lapack =  boost::numeric::bindings::lapack;

class ArnoldiTest : public Arnoldi {
    public:
        ArnoldiTest(bool v = true, 
                double Tol = std::numeric_limits<double>::epsilon() )
            : Arnoldi(v, Tol){   }

        void Orthogonalize();
        void Normalize();
        void CalcEigenPairs();

    private:
        Arnoldi::cVec_t _Av(){    }
        void _Iteration(){  }
        void _Init_V(unsigned int, unsigned int);
        void _Init_H(unsigned int, unsigned int);

        void _IRestart(cVec_t&){    }    // Implicit Restart
        void _ERestart(cVec_t&){    }    // Explicit Restart

};

void ArnoldiTest::Normalize(){
    cout << "====================================\n"
         << "Checking Normalization." << endl;
    _length = 5;
    _H.resize( _length+1, _length );

    Arnoldi::cVec_t q(_length);
    for( int i = 0; i < q.size(); ++i ) q(i) = i+1;
    print_v(q,"q");

    _k = 3;
    cout << "_k = " << _k << endl;
    _Normalize(q);
    print_v(q, "q");
    print_m(_H, "_H");
}

void ArnoldiTest::Orthogonalize(){
    cout << "====================================\n"
         << "Checking Orthogonalize." << endl;
    _length = 5;
    _k = 2;
    _H.resize( _length+1, _length );
    cout << "_k = " << _k << ", _length = " << _length << endl;
    print_m(_H,"_H");
    cout << endl;

    _Init_V(_length, _k+1);
    print_m(_V, "_V");
    cout << endl;

    Arnoldi::cVec_t q(_length);
    for( int i = 0; i < q.size(); ++i ) q(i) = i+1;
    print_v(q,"q-before");
    _Orthogonalize(q);
    print_v(q,"q-orthoged");
    print_m(_H, "_H");
}

void ArnoldiTest::CalcEigenPairs(){
    cout << "====================================\n"
         << "Checking CalcEigenPairs." << endl;
    unsigned int I = 5;
    _H.resize(I+1, I);
    _H *= 0;
    Matrices::Standard(_H);
    print_m(_H, "_H");

    _k = I;
    cout << "_k = " << _k << endl;
    _CalcEigenPairs();
    print_v(Eigenvalues(), "Values");
    print_m(Eigenvectors(), "EigenVectors");
    cout << endl;
    cout << "Subtracting l[i]*x[i] from _H*x[i], where l[i] is an eigenvalue\n"
         << "and x[i] is the associated eigenvector.  If calculation is\n"
         << "correct, the following values should be zero." << endl;
    cMat_t H_tmp = ublas::subrange(_H, 0,_k, 0,_k);   // Submatrix of _H
    for( int i = 0; i < _Eigenvectors.size2(); ++i ){
        ublas::vector<complex<double> > tmp(_k);
        tmp = ublas::prod( H_tmp, column(_Eigenvectors, i) );
        cout << tmp - _Eigenvalues(i)*column(_Eigenvectors,i) << endl;
    }
    cout << "\n-----------\n" << endl;
}

void ArnoldiTest::_Init_V(unsigned int r, unsigned int c){
    _V.resize(r,c);
    Matrices ::Full(_V);
    for( int i = 0, end = _V.size2(); i != end; ++i ){
        cColumn_t Qi(_V, i);
        Qi /= ublas::norm_2(Qi);
    }
}

#endif

