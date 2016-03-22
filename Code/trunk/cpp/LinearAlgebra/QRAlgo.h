/*QRAlgo.h
 $Author: jlconlin $
 $Id: QRAlgo.h 522 2009-04-24 13:35:29Z jlconlin $
 $Revision: 522 $
 $Date: 2009-04-24 07:35:29 -0600 (Fri, 24 Apr 2009) $

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
#include "boost/numeric/bindings/lapack/orgqr.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

template<typename M, typename S>
inline void QRStepShifted(M&, M&, S );
template<typename M>
void QRStepShifted(M&, M&, std::complex<double>, int&);
template<typename M>
void QRStepShifted(M&, M&, double, int&);

template <typename T>
struct transpose {
   static const char value ;
};

template <typename T>
const char transpose<T>::value = 'T';

template <typename T>
struct transpose< std::complex<T> > {
   static const char value ;
};

template <typename T>
const char transpose< std::complex<T> >::value = 'C';


/*QR is the QR algorithm.  It performs the same number of QR steps as there are
 * shifts.
 * A: Matrix for QR factorization
 * Q: Return of orthogonal matrix Q = Q1*Q2*...Qn  On input it is usually the 
 *      identity matrix
 * Shifts: Shifts (clearly)*/
template<typename M, typename S>
void QRShifted(M& A, M& Q, S& Shifts){
    unsigned int n = Shifts.size();
    for( int i = 0; i < n; ++i ){
//      cout << "\nShift = " << Shifts[i] << endl;
//      print_m(A, "A"); cout << endl;
//      print_m(Q, "Q"); cout << endl;
        QRStepShifted( A, Q, Shifts[i], i );
    }
}

// To satisfy the compiler
template<typename M, typename S>
inline void QRStepShifted(M& A, M& Q, S Shift){
    int ind = 0;
    QRStepShifted(A, Q, Shift, ind);
}

/*QRStep_Shifted will perform one shifted QR step.  If the shift has a 
 * significant imaginary component it will do a double shift.  It overwrites 
 * the matrix A by Q^T*A*Q where Q is the Q in the QR factorization.
 * The double shift algorithm comes from:
 *      "Matrix Computataions 3rd Edition" by Gene Golub and Charles Van Loan
 *      pp. 355-356.
 *
 * index: The index of the Shifts.  Required when doing double-shift QR.*/
template<typename M>
void QRStepShifted(M& A, M& Q, std::complex<double> Shift, int& index){
    typedef typename M::value_type value_type;
    typedef typename M::size_type size_type;

    size_type n( A.size1() );
    ublas::identity_matrix<value_type, ublas::column_major> I( n, A.size2() );
    M m( A );

    if( Shift.imag() != 0 ){ // Perform double shift
        double s = -2*Shift.real();
        double t = pow(Shift.real(), 2) + pow(Shift.imag(), 2);
        m = prod(A, A);
        m += s*A + t*I;
        index += 1;     // Let calling function know we performed two shifts
    }
    else{   // Ignoring imaginary part
        m -= Shift.real()*I;
    }

    QR(A, m, Q );
    
    // Clean up below subdiagonal elements
//  for( int i = 0; i < n; ++i ){
//      for ( int j = 0; j < n; ++j ) if( i > j+1 ) A(i,j) = 0;
//  }

}

/*QRStepShifted will perform one shifted QR step.*/
template<typename M>
void QRStepShifted(M& A, M& Q, double Shift, int& index ){
    typedef typename M::value_type value_type;
    typedef typename M::size_type size_type;

    size_type n( A.size1() );
    ublas::identity_matrix<value_type, ublas::column_major> I( n, A.size2() );
    
    M m( A );
    m -= Shift*I;

    QR(A, m, Q);
//  cout << "Shift: " << Shift << endl;
//  print_m(A, "A-after");  cout << endl;
}

template<typename M>
void QR(M& A, M& m, M& Q){
    typedef typename M::value_type value_type;
    typedef typename M::size_type size_type;

    ublas::vector<value_type> tau(A.size1() );

//  print_m(m, "m");    cout << endl;
    //Compute QR factorization
    lapack::geqrf( m, tau, lapack::optimal_workspace() );

    // Make orthogonal matrix Q
    lapack::orgqr( m, tau, lapack::optimal_workspace() );
    A = prod(trans(m), A); A = prod(A, m);
    Q = prod(Q, m);
//  print_m(m, "Q");  cout << endl;
//  print_m(A, "A-after Q^T*A*Q");  cout << endl;

//  cout << "===================" << endl;

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
    lapack::geqrf( A, tau, lapack::optimal_workspace() );

    //Apply the orthogonal transformations to A
    // Upper triangle of A is R
    ublas::triangular_adaptor<M, ublas::upper> C(A); 
    M R(C);
    lapack::ormqr('R', 'N', A, tau, R, lapack::optimal_workspace() );
}

#endif

