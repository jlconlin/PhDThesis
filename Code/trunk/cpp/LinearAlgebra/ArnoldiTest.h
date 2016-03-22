/*ArnoldiTest.h
 $Author: jlconlin $
 $Id: ArnoldiTest.h 522 2009-04-24 13:35:29Z jlconlin $
 $Revision: 522 $
 $Date: 2009-04-24 07:35:29 -0600 (Fri, 24 Apr 2009) $

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

class ArnoldiTest : public Arnoldi<double> {
    public:
        ArnoldiTest(bool v = true, 
                double Tol = std::numeric_limits<double>::epsilon() )
            : Arnoldi<double>(v, Tol){   }

        void Orthogonalize();
        void Normalize();
        void CalcEigenPairs();
        void SortEigenPairs();

    private:
        T_vec_t _Av(){    }
        void _Iteration(T_vec_t&){  }
        void _Init_V(unsigned int, unsigned int);
        void _Init_H(unsigned int, unsigned int);

        void _Restart(T_vec_t&){    }    // Restart

};

void ArnoldiTest::Normalize(){
    cout << "====================================\n"
         << "Checking Normalization." << endl;
    _length = 5;
    _H.resize( _length+1, _length );

    T_vec_t q(_length);
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

    T_vec_t q(_length);
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
    T_mat_t H_tmp = ublas::subrange(_H, 0,_k, 0,_k);   // Submatrix of _H
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
        T_column_t Qi(_V, i);
        Qi /= ublas::norm_2(Qi);
    }
}


void ArnoldiTest::SortEigenPairs(){
    cout << "====================================\n"
         << "Checking SortEigenPairs." << endl;

    unsigned int n(5);
    _length = 6;
    c_vec_t Values(n);
    c_vec_t Ones(n, complex<double>(1.0,0.0) );
    c_mat_t Vectors(_length, n);

    // Initialize eigenvalues and eigenvectors
    Values[0] = complex<double>(1.0,0.0); Values[1] = complex<double>(4.0,0.0);
    Values[2] = complex<double>(2.0,0.0); Values[3] = complex<double>(3.0,1.0);
    Values[4] = complex<double>(3.0,2.0); 

    for( int i = 0; i < n; ++i ){
        c_column_t vec(Vectors, i);
        for( int j = 0; j < _length; ++j ){
            vec[j] += complex<double>(i+1,0.0);
        }
    }
    print_v(Values, "Eigenvalues" );
    print_m(Vectors, "Eigenvectors");

    _SortEigenPairs(Values, Vectors);

    cout << "Sorted Eigenpairs:" << endl;
    print_v(_Eigenvalues, "Eigenvalues" );
    print_m(_Eigenvectors, "Eigenvectors");

}

#endif

