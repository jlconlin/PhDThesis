/*QRAlgo.h
 $Author: jlconlin $
 $Id: QRAlgo.h 340 2008-07-30 21:02:36Z jlconlin $
 $Revision: 340 $
 $Date: 2008-07-30 15:02:36 -0600 (Wed, 30 Jul 2008) $

 This header file provides some functions required for the QR Algorithm.  It 
 was designed to be used with Implicitly Restarted Arnoldi's Method.
 */

#ifndef QRALGO_H
#define QRALGO_H

#include<iostream>
#include "boost/numeric/ublas/triangular.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/bindings/lapack/geqrf.hpp"
#include "boost/numeric/bindings/lapack/ormqr.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

/*QR is the QR algorithm.  It performs the same number of QR steps as there are
 * shifts.
 * A: Matrix for QR factorization
 * Q: Return of orthogonal matrix Q = Q1*Q2*...Qn  On input it is usually the 
 *      identity matrix
 * Shifts: Shifts (clearly)*/
template<typename M, typename S>
void QRShifted(M& A, M& Q, S& Shifts){
    typename S::iterator shiftIter = Shifts.begin();
    for( ; shiftIter != Shifts.end(); ++shiftIter ){
        QRStepShifted(A, Q, *shiftIter);
    }
}

/*QRStep_Shifted will perform one shifted QR step.  It overwrites the matrix A
 * with the new A' = RQ + sI where s is the shift, I is the Identity matrix and
 * QR = A - sI is the shifted QR factorization.*/
template<typename M, typename S>
void QRStepShifted(M& A, M& Q, S Shift){
    typedef typename M::value_type value_type;
    typedef typename M::size_type size_type;

    size_type n( A.size1() );
    ublas::vector<value_type> tau(n);
    ublas::identity_matrix<value_type, ublas::column_major> I( n, A.size2() );
    A -= Shift*I;

    //Compute QR factorization
    lapack::geqrf( A, tau, lapack::optimal_workspace() );

    //Apply the orthogonal transformations to A
    // Upper triangle of A is R
    ublas::triangular_adaptor<M, ublas::upper> C(A); M R(C);
    lapack::ormqr('R', 'N', A, tau, R, lapack::optimal_workspace() );
    // Make orthogonal matrix Q
    lapack::ormqr('R', 'N', A, tau, Q, lapack::optimal_workspace() );

    A = R + Shift*I;

//  print_m(Q, "Q");
//  print_m(A, "A");
}

/*QRStep will perform one complete step in the QR algorithm.  It overwrites
 * matrix A with the new A' = RQ where QR = A is the QR factorization.*/
template<typename M>
void QRStep(M& A){
    typedef typename M::value_type value_type;
    typedef typename M::size_type size_type;

    size_type n( A.size1() );
    ublas::vector<value_type> tau(n);

    // Compute QR factorization
    cout << "Compute QR factorization." << endl;
    lapack::geqrf( A, tau, lapack::optimal_workspace() );
    print_m(A, "A"); cout << endl;
    print_v(tau, "tau"); cout << endl;

    //Apply the orthogonal transformations to A
    // Upper triangle of A is R
    ublas::triangular_adaptor<M, ublas::upper> C(A); 
    M R(C);
    print_m(R, "R"); cout << endl;
    lapack::ormqr('R', 'N', A, tau, R, lapack::optimal_workspace() );
    print_m(A, "A"); cout << endl;
    print_m(R, "R"); cout << endl;
    print_v(tau, "tau"); cout << endl;
}

#endif

