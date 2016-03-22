/*QRAlgoTest.cpp
 $Author: jlconlin $
 $Id: QRAlgoTest.cpp 340 2008-07-30 21:02:36Z jlconlin $
 $Revision: 340 $
 $Date: 2008-07-30 15:02:36 -0600 (Wed, 30 Jul 2008) $

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

namespace ublas = boost::numeric::ublas;

int main(){
    cout << "Testing QR Algorithm functions." << endl;
    
    cout << "\n==========================================================\n"
         << "Comparing my QR Algorithm with results in Watkins pg. 363." 
         << endl;

    unsigned int n = 5;
    ublas::matrix<double, ublas::column_major> A(n,n);
//  Matrices::Watkins(A);
//  ublas::vector<double> Shifts(3);
//  Shifts(0) = 5.0; Shifts(1) = 4.07692; Shifts(2) = 4.00001907341357;
//  Matrices::Watkins(A);
//  QRStep(A);

    Matrices::Moler(A);
    ublas::vector<double> Shifts(6);
    Shifts(0) = -25; Shifts(1) = -14.1578; Shifts(2) = -6.06587;
    Shifts(3) = -1.82436; Shifts(4) = 0.305671; Shifts(5) = 1.3455;
    print_m(A, "A");
    print_v(Shifts, "Shifts");

    ublas::identity_matrix<double, ublas::column_major> I( n, A.size2() );
    ublas::matrix<double, ublas::column_major> Q(I);
    QRShifted(A, Q, Shifts);
    print_m(A, "A");
    print_m(Q, "Q");

    cout << "\nepsilon: " << std::numeric_limits<double>::epsilon() << endl;

    return 0;
}

