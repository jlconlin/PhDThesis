/*QRAlgoTest.cpp
 $Author: jlconlin $
 $Id: QRAlgoTest.cpp 528 2009-04-29 20:14:39Z jlconlin $
 $Revision: 528 $
 $Date: 2009-04-29 14:14:39 -0600 (Wed, 29 Apr 2009) $

 This is used to test the QR Algorithm functions contained in QRAlgo.h
 */

#include "QRAlgo.h"

#include<iostream>
#include<limits>
#include "Matrices.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::complex;

namespace ublas = boost::numeric::ublas;

void Watkins();
void Hessenberg();

int main(){
    cout << "Testing QR Algorithm functions." << endl;
    
    Watkins();
//  Hessenberg();
    return 0;
}
void Watkins( ){
    cout << "\n==========================================================\n"
         << "Comparing my QR Algorithm with results in Watkins pg. 363." 
         << endl;

    int n = 2;
    ublas::matrix<double, ublas::column_major> A(n,n);
    ublas::identity_matrix<double, ublas::column_major> I( n, n );
    ublas::matrix<double, ublas::column_major> Q(I);

    Matrices::Watkins(A);
    print_m(A, "A");

    ublas::vector<double> Shifts(4);
    Shifts(0) = 5.0; Shifts(1) = 4.07692; 
    Shifts(2) = 4.00001907341357; Shifts(3) = 4.0;
    print_v(Shifts, "Shifts");

    cout << "Performing four shifts to see if it matches Watkins." << endl;
    int j = 0;
    for(int i = 0; i < Shifts.size(); ++i ){
        QRStepShifted(A, Q, Shifts[i] );
        print_m(A, "A");
    }
}

void Hessenberg(){
    cout << "\n==========================================================\n"
         << "Investigating double-shifted QR algorithm."
         << endl;

    int n = 5;
    ublas::matrix<double, ublas::column_major> A(n,n);
    ublas::identity_matrix<double, ublas::column_major> I( n, n );
    ublas::matrix<double, ublas::column_major> Q(I);

    Matrices::UHessenberg(A);
    print_m(A, "A");
    ublas::vector<complex<double> > Shifts(n);
    Shifts(0) = complex<double>(-1.50269284,2.13744141);
    Shifts(1) = complex<double>(-1.50269284,-2.13744141); 
    Shifts(2) = 3.68123406;
    Shifts(3) = 10.90610875;
    Shifts(4) = 43.41804288;
    print_v(Shifts, "Shifts");

    QRShifted(A, Q, Shifts);

    print_m(A, "A");
    print_m(Q, "Q");
}

