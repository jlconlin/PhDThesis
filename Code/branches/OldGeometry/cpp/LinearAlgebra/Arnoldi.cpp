/*Arnoldi.cpp
 $Author: jlconlin $
 $Id: Arnoldi.cpp 349 2008-08-07 18:32:50Z jlconlin $
 $Revision: 349 $
 $Date: 2008-08-07 12:32:50 -0600 (Thu, 07 Aug 2008) $

 The Arnoldi class is the C++ implementation of Arnoldi's method.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <complex>
#include <utility>
#include "Utilities.h"
#include "Arnoldi.h"
#include "QRAlgo.h"
#include "boost/compressed_pair.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/bindings/lapack/geev.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_vector.hpp"

#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::complex;

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

/*IRAM is Implicitly Restarted Arnoldi's Method.  It is patterned after Danny 
 * Sorensen's MATLAB implementation
 *
 * q: Starting vector
 * n: Number of desired eigenvalues
 * I: Total number of Arnoldi iterations
 * R: Number of Arnoldi Restarts*/
void Arnoldi::IRAM(dVec_t q, unsigned int n, unsigned int I, unsigned int R){

    _I = I;
    _n = n;
    cVec_t v(q);

    // Start timing
    _start = boost::date_time::microsec_clock<ptime>::local_time();

    // Do Restarts
    _stoppedR = false;
    _R = 0;
    while( not _stoppedR ){
        _IRestart(v);

        ++_R;
        if( _R >= R ) _stoppedR = true;
    }
}

/*ERAM is Explicitly Restarted Arnoldi's Method.  It performs explicit 
 * restarts as proposed by Y. Saad.
 *
 * q: Starting vector
 * n: Number of desired eigenvalues
 * I: Number of Arnoldi iterations
 * R: Number of Restarts*/
void Arnoldi::ERAM(dVec_t q, unsigned int n, unsigned int I, unsigned int R){

    _I = I;
    _n = n;
    cVec_t v(q);

    // Start timing
    _start = boost::date_time::microsec_clock<ptime>::local_time();

    // Do Restarts
    _stoppedR = false;
    _R = 0;
    while( not _stoppedR ){
        _ERestart(v);

        ++_R;
        if( _R >= R ) _stoppedR = true;
    }
}

/*arnoldi is the main method of this class.  This is just a basic function.  It
 * doesn't do anything other than create the Arnoldi factorization given the 
 * starting vector v and a matrix-vector product defined in _Av()
 * v: Initial starting vector
 * I: Number of Arnoldi Iterations to perform.*/
void Arnoldi::arnoldi(cVec_t& v, unsigned int I){
    if( not _k ){
        _length = v.size();
        _H.resize( I, I );    _H *= 0.0;
        _V.resize( _length, I );  _V *= 0.0;

        v /= ublas::norm_2(v);  // Normalize v
        cColumn_t q = cColumn_t(_V, 0);
        q = v;   // Set first column of _V to be starting vector

        // Orthogonalize first vector
        v = _Av();
        complex<double> alpha = inner_prod(q,v);
        v -= q*alpha;
        _H(0,0) = alpha;

        _k = 1;
    }

    _stoppedI = false;
    while( not _stoppedI ){
        _Iteration(v);

        ++_k;
        if( _k >= I ) _stoppedI = true;
    }   // while( not_stopped )
}

/*_IRestart manages all the things that much happen at each Restart.*/
void Arnoldi::_IRestart(cVec_t& v){
    // Start timing restart
    _startR = boost::date_time::microsec_clock<ptime>::local_time();
    
    arnoldi(v, _I);
    _CalcEigenPairs();
    _MakeRitzPairs();

    // Calculate residual
    _residual = abs(_Eigenvectors(_k-1, _k-1))*ublas::norm_2(v);

    // Do shifted QR algorithm
    ublas::identity_matrix<complex<double>, ublas::column_major> Id(_I, _I);
    cMat_t Q( Id );
    cVec_t Shifts( ublas::subrange( _Eigenvalues, 0, _n ) );
    cMat_t Hm( ublas::subrange( _H, 0,_I, 0,_I ) );
    QRShifted( Hm, Q, Shifts);

    // Update _k, _H, _V
    _k = _n;
    ublas::subrange( _H, 0,_I, 0,_I ) = Hm;
    ublas::subrange( _V, 0,_length, 0,_n ) = prod( 
        ublas::subrange( _V, 0,_length, 0,_I ), 
        ublas::subrange( Q, 0,_I, 0,_n ) );

    // Update v for next restart
    v = v*Q(_I-1, _n-1);
}

/*_ERestart manages all the things that must happen at each explicit Restart.*/
void Arnoldi::_ERestart(cVec_t& v){
    arnoldi(v, _I);
    _CalcEigenPairs();
    _MakeRitzPairs();

    // Calculate residual
    _residual = abs(_Eigenvectors(_k-1, _k-1))*ublas::norm_2(v);

    // Update v for next restart
    v *= 0;
    for( int i  = 0; i < _n; ++i ){
        v += cColumn_t( _Ritzvectors, (_k-i) );
    }

}

/*_Iteration manages all the things that must happen at each iteration.*/
void Arnoldi::_Iteration(cVec_t& v){
    _Normalize(v);
    cColumn_t(_V, _k) = v;    // Add v to _V

    v = _Av();
    _Orthogonalize(v);
}

/*_Orthogonalize will orthogonalize vector q against all previously calculated
 * Arnoldi vectors.*/
void Arnoldi::_Orthogonalize(cVec_t& v){

    cMat_t sub_V( ublas::subrange( _V, 0,_length , 0, _k+1) );
    cVec_t h = prod( trans(sub_V), v);
      
    // Assign h to column of _H (this looks nasty but that's just the way it is)
    ublas::matrix_vector_slice<cMat_t>( _H, 
            ublas::slice(0,1, _k+1), ublas::slice(_k, 0, _k+1) ) = h;
    v = v - prod(sub_V, h);
}

void Arnoldi::_Normalize(cVec_t& q){
    complex<double> NormFactor = ublas::norm_2(q);
    _H(_k,_k-1) = NormFactor;
    q /= NormFactor;
}

/*_CalcEigenPairs will calculate the Eigenpairs of the upper Hessenberg 
 * matrix.*/
void Arnoldi::_CalcEigenPairs(){
    cVec_t Values(_k);
    cMat_t Vectors(_k, _k);

    cMat_t H_tmp = ublas::subrange(_H, 0,_k, 0,_k);   // Submatrix of _H
    cMat_t* LeftVectors = 0;

    lapack::geev(H_tmp, Values, LeftVectors, &Vectors, 
            lapack::optimal_workspace() );  // Calculate Eigenpairs

    NormColumns(Vectors); // Normalize Vectors

    _SortEigenPairs(Values, Vectors);
}

/*_MakeRitzPairs will create the Ritzpairs of the linear operator.
 * This one is easy :D.*/
void Arnoldi::_MakeRitzPairs(){
    cMat_t sub_V(ublas::subrange(_V, 0,_length, 0,_I));

    _Ritzvectors = ublas::prod(sub_V,
            _Eigenvectors);
//  print_m(sub_V, "sub_V");
//  print_m(_Eigenvectors, "_Eigenvectors");
//  print_m(_Ritzvectors, "_Ritzvectors");
}

/*Compare will return whether x is less than y defined by the real parts of the
 * first elements of the pair.  If the real part is equal the imaginary parts
 * are compared.*/
inline bool Compare(Arnoldi::ePair const x, Arnoldi::ePair const y){
    if( x.first.real() == y.first.real() ){
       return x.first.imag() < y.first.imag();
    }
    else  return x.first.real() < y.first.real();
}

/*_SortEigenPairs will sort _Eigenvalues and associated eigenvectors by size
 * of the eigenvalue.
 * Vectors: Eigenvectors associated with _Eigenvalues.  Assumed to be in same
 *  order as eigenvalues.*/
void Arnoldi::_SortEigenPairs(cVec_t& Values, cMat_t& Vectors){

    std::vector<ePair> Pairs( Values.size() );
    std::vector<ePair>::iterator pairIter = Pairs.begin();

    // Make vector of eigenvalues and indices
    unsigned int index = 0;
    cVec_t::iterator valIter = Values.begin();
    for( ; valIter != Values.end(); ++valIter, ++pairIter, ++index ){
        *pairIter = ePair(*valIter, index);
    }

    std::stable_sort(Pairs.begin(), Pairs.end(), &Compare);

    _Eigenvalues.resize(_k);    _Eigenvectors.resize(_k,_k);
    valIter = _Eigenvalues.begin();
    pairIter = Pairs.begin();
    int i = 0;
    for( ; pairIter != Pairs.end(); ++pairIter, ++valIter, ++i ){
        index = pairIter->second;
        *valIter = Values(index);
        cColumn_t(_Eigenvectors, i) = cColumn_t(Vectors, index);
    }
}
