/*ArnoldiMatrixTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiMatrixTest.cpp 340 2008-07-30 21:02:36Z jlconlin $
 $Revision: 340 $
 $Date: 2008-07-30 15:02:36 -0600 (Wed, 30 Jul 2008) $

 This is used to test the ArnoldiMatrix class
 */

#include<iostream>
#include "ArnoldiMatrixTest.h"
#include "ArnoldiMatrix.h"
#include "Matrices.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::complex;

namespace ublas =  boost::numeric::ublas;

int main( int argc, char* argv[] ){

    cout << "I'm testing ArnoldiMatrix class." << endl;
    
    unsigned int n = 5;
    unsigned int r = n;
    unsigned int c = n;

    ublas::matrix<double, ublas::column_major> A(r,c);
    Matrices::Standard(A);

    cout << "====================================\n"
         << "Creating ArnoldiMatrixTest object." << endl;
    ArnoldiMatrixTest arnMatTest(A);
    print_m( arnMatTest.Matrix(), "arnMat Matrix" );

//  arnMatTest.arnoldi();
    arnMatTest.IRAM();
    return 0;
}

