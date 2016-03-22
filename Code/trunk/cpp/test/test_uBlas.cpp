/*test_uBlas.cpp
 $Author: jlconlin $
 $Id: test_uBlas.cpp 374 2008-09-09 20:44:16Z jlconlin $
 $Revision: 374 $
 $Date: 2008-09-09 14:44:16 -0600 (Tue, 09 Sep 2008) $

 This file is used for testing/learning about Boost.org uBlas library.

 */

#include<iostream>
#include<cstdlib>
#include<complex>
#include<cmath>
#include "Matrices.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/triangular.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_vector.hpp"
#include "boost/numeric/bindings/lapack/geev.hpp"
#include "boost/numeric/bindings/lapack/hseqr.hpp"
#include "boost/numeric/bindings/lapack/trevc.hpp"
#include "boost/numeric/bindings/lapack/geqrf.hpp"
#include "boost/numeric/bindings/lapack/ormqr.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::vector;
using std::complex;

namespace ublas =  boost::numeric::ublas;
namespace lapack =  boost::numeric::bindings::lapack;

void basic(int);
void geev(int);
void hseqr(int);
void hseqr_trevc(int);
void geqrf_ormqr_orgqr(int);
template <typename T>
void NormColumns(ublas::matrix<T, ublas::column_major>& );

int main(){
    cout << "I'm testing uBlas." << endl;

    ublas::vector<double> v(3);
    v[0] = 2; v[1] = 3; v[2] = 5;
    print_v(element_prod(v,v), "s");

    ublas::vector<complex<double> > c(3);
    c[0] = complex<double>(1.0,2.0);
    c[1] = complex<double>(3.0,4.0);
    c[2] = complex<double>(5.0,6.0);
    print_v(c, "complex vector");
    v = real(c);
    print_v(v, "Real part");
    print_v(element_prod(c,c), "c^2");
//  v = std::abs(c);
//  print_v(v, "Abs");

    int n = 5;
    basic(n);
//  geev(n);
//  hseqr(n);
//  hseqr_trevc(n);
//  geqrf_ormqr_orgqr(n);

}

void geqrf_ormqr_orgqr(int n){
    cout << "\n===============================================\n" 
         << "Testing QR functionality" << endl;

    typedef ublas::matrix<double, ublas::column_major> M;
    typedef M::value_type value_type;


    M A(n,n);
    Matrices::Moler(A);
    ublas::vector<value_type> tau(n);
    ublas::identity_matrix<value_type, ublas::column_major> I( n, n );

    //Compute QR factorization
    lapack::geqrf( A, tau, lapack::optimal_workspace() );
    //Apply the orthogonal transformations to A
    // Upper triangle of A is R
    ublas::triangular_adaptor<M, ublas::upper> C(A); M R(C);
    print_m(R, "R");

    // Make Q
    M Q(I);
    lapack::ormqr('R', 'N', A, tau, Q, lapack::optimal_workspace() );
//  M Q(A);
//  lapack::orgqr( Q, tau );
    print_m(Q, "Q");

}

void basic(int n){
    typedef ublas::matrix<double, ublas::column_major> Mat_t;
    int m = 6;

    cout << "\n'Regular' matrix." << endl;
    Mat_t A(m,n);
    Matrices::UHessenberg(A);
    cout << "Here is my matrix A:\n" << endl;
    print_m(A, "A");

    cout << "Here is a 2x2 submatrix of A:\n";
    ublas::matrix_range<Mat_t > mr(A,
            ublas::range(1,3),ublas::range(2,4) );
    print_m(mr, "mr");

    cout << "\nHere is the upper left 3x4 matrix of A:" << endl;
    Mat_t subA(subrange(A, 0,3, 0,4 ) );
    print_m(subA, "subA");
    cout << "\nHere is the lower right 2x3 matrix of A:" << endl;
    subA = subrange(A, A.size1()-2,A.size1(), A.size2()-3,A.size2() );
    print_m(subA, "subA");

//  cout << "\n===============================\n" 
//       << "vector-vector product.\n" << endl;
//  ublas::vector<double> v(n);
//  ublas::vector<double> u(n);
//  int k = 0;
//  for( int i = 0; i < v.size(); ++i ) v(i) = ++k;
//  for( int i = 0; i < u.size(); ++i ) u(i) = ++k;
//  print_v(v, "v");
//  print_v(u, "u");
//  cout << "v.u = " << inner_prod(v,u) << endl;
//  cout << "v + 3*u = "; print_v(v+3*u);
//  cout << "u /= 2 = "; print_v(u/=2);

//  cout << "\n===============================\n" 
//       << "Matrix-vector product.\n" << endl;
//  cout << endl;

//  u.resize(m);
//  u = prod(A,v);
//  print_v( u, "u");
//  
//  cout << "\n===============================\n" 
//       << "Replacing column with a vector.\n" << endl;
//  print_m(A, "A"); cout << endl;
//  ublas::matrix_column<Mat_t > column(A,0);
//  column = u;
//  print_m(A,"A");

    cout << "\n===============================\n" 
         << "Taking a vector slice of a matrix.\n" << endl;
    Matrices::Full(A);
    print_m(A,"A");
    cout << "\nFirst two elements of first column." << endl;
    ublas::matrix_vector_slice<Mat_t> sl1(A, 
        ublas::slice(0,1,2), ublas::slice(0,0,2));
    print_v(sl1);

    cout << "\nSecond through fourth elements of third column." << endl;
    ublas::matrix_vector_slice<Mat_t> sl2(A, 
            ublas::slice(1,1,3), ublas::slice(2,0,3));
    print_v(sl2);

    cout << "\nFirst through fifth elements of second column." << endl;
    ublas::matrix_vector_slice<Mat_t> sl3(A, 
            ublas::slice(0,1,5), ublas::slice(1,0,5));
    print_v(sl3);

    cout << "\nFirst through fifth elements of fourth column." << endl;
    ublas::matrix_vector_slice<Mat_t> sl4(A, 
            ublas::slice(0,1,5), ublas::slice(3,0,5));
    print_v(sl4);

    cout << "\nblah blah blah." << endl;
    ublas::matrix_vector_slice<Mat_t> sl5(A, 
            ublas::slice(0,1,3), ublas::slice(0,1,3));
    print_v(sl5);
    
}

void hseqr(int n){
    cout << "\nCalculating eigenvalues using LAPACK's hseqr." << endl;
    ublas::matrix<double, ublas::column_major> H(n,n);
    Matrices::UHessenberg(H);
    cout << "\nUpper Hessenberg matrix H:\n" << H << endl;

    ublas::vector<complex<double> > values(n);
    ublas::matrix<double, ublas::column_major> Z(n,n);

    cout << "\nHSEQR for only eigenvalues." << endl;
    lapack::hseqr('E', H, values);
    cout << "\nH:\n" << H << endl;
    cout << "\nvalues: " << values << endl;

    cout << "\nHSEQR for eigenvalues and Schur vectors." << endl;
    Matrices::UHessenberg(H);
    cout << "H:\n" << H << endl;
    lapack::hseqr('S', 'I', H, values, Z);
    cout << "\nH: " << H << endl;
    cout << "\nvalues: " << values << endl;
    cout << "\nZ: " << Z << endl;

    cout << "\n==================================" << endl;
    cout << "Recalculating original matrix..." << endl;
    ublas::matrix<double, ublas::column_major> cH(n,n);
    cH = ublas::prod( H, ublas::herm(Z) );
    cH = ublas::prod( Z, cH );
    cout << "'New' original matrix:\n" << cH << endl;
    cout << "==================================" << endl;


    cout << "\nHSEQR for only eigenvalues.  Complex version" << endl;
    ublas::matrix<complex<double>, ublas::column_major> G(n,n);
    Matrices::UHessenberg(G);
    cout << "\nG:\n" << G << endl;
    lapack::hseqr('E', G, values);
    cout << "\nG:\n" << G << endl;
    cout << "\nvalues: " << values << endl;

    cout << "\nHSEQR for eigenvalues and Schur vectors." << endl;
    Matrices::UHessenberg(G);
    cout << "G:\n" << G << endl;
    ublas::matrix<complex<double>, ublas::column_major> cZ(n,n);
    lapack::hseqr('S', 'I', G, values, cZ);
    cout << "\nG:\n " << G << endl;
    cout << "\nvalues: " << values << endl;
    cout << "\nZ:\n " << Z << endl;

    cout << "\n==================================" << endl;
    cout << "Recalculating original matrix..." << endl;
    ublas::matrix<complex<double>, ublas::column_major> origG(G);
    origG = ublas::prod( G, ublas::herm(cZ) );
    origG = ublas::prod( cZ, origG );
    cout << "'New' original matrix:\n" << origG << endl;
    cout << "==================================" << endl;

}

void hseqr_trevc(int n){
    cout << "\nLearning in hseqr_trevc." << endl;
    ublas::matrix<complex<double>, ublas::column_major> H(n,n);
    Matrices::UHessenberg(H);
//  Matrices::Diagonal(H);
//  Matrices::Standard(H);
    cout << "\nUpper Hessenberg matrix H:\n" << endl;
    print_m(H, "H");

    typedef ublas::matrix<complex<double>, ublas::column_major> Mat_t;
    typedef ublas::matrix_row<Mat_t> Row_t;
    typedef ublas::matrix_column<Mat_t> Column_t;
    typedef Mat_t::size_type size_t;

    ublas::matrix_range<Mat_t> mr(H,
            ublas::range(0,n-2),ublas::range(0,n-2) );
    ublas::vector<complex<double> > values(n);
    Mat_t Z(n,n);

    int result = lapack::hseqr('S', 'I', H, values, Z);

    cout << "hseqr result = " << result << endl;
    print_v(values, "values");
    print_m(H, "H->T");
    print_m(Z, "Schur vectors");

    ublas::vector<complex<double> > Work(3*n);
    Mat_t vectors(Z);

    result = lapack::trevc('R', 'B', H,  vectors, vectors, Work );

    cout << "trevc result = " << result << endl;
    print_m(vectors,"raw-vectors");
    NormColumns(vectors);
    print_m(vectors, "normed-vectors");

    cout << "T*x = l*x." << endl;
    for( int i = 0; i < vectors.size2(); ++i ){
        ublas::vector<complex<double> > tmp(n);
        tmp = ublas::prod( H, column(vectors, i) );
        cout << tmp - values(i)*column(vectors,i) << endl;
    }
    cout << endl;
    Mat_t origH(n,n); Matrices::UHessenberg(origH);
    cout << "H*x = l*x." << endl;
    for( int i = 0; i < vectors.size2(); ++i ){
        ublas::vector<complex<double> > tmp(n);
        tmp = ublas::prod( origH, column(vectors, i) );
        cout << tmp - values(i)*column(vectors,i) << endl;
    }
}

void geev(int n){
    cout << "\nCalculating eigenvalues using LAPACK's geev." << endl;
    ublas::matrix<double, ublas::column_major> A(n,n);
    Matrices::UHessenberg(A);
    print_m(A);

    ublas::vector<complex<double> > values(n);
    ublas::matrix<complex<double>, ublas::column_major>* Vectors_left = 0;
    ublas::matrix<complex<double>, ublas::column_major> Vectors_right(n,n);

    lapack::geev(A, values, Vectors_left, &Vectors_right, lapack::optimal_workspace());
    print_v(values, "values"); cout << endl;
    print_m(Vectors_right, "Vectors_right"); cout << endl;

    Matrices::UHessenberg(A);
    cout << "A*x = l*x." << endl;
    for( int i = 0; i < Vectors_right.size2(); ++i ){
        ublas::vector<complex<double> > tmp(n);
        tmp = ublas::prod( A, column(Vectors_right, i) );
        cout << tmp - values(i)*column(Vectors_right,i) << endl;
    }

}

template <typename T>
void NormColumns(ublas::matrix<T, ublas::column_major>& A){
    typedef ublas::matrix<complex<double>, ublas::column_major> Mat_t;
    typedef ublas::matrix_column<Mat_t> Column_t;
    typedef Mat_t::size_type size_t;

    for( size_t index = 0, end = A.size2(); index != end; ++index){
        Column_t column( A,index );
        column /= ublas::norm_2(column);
    }
}


