/*ArnoldiMatrixTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiMatrixTest.cpp 284 2008-04-09 13:33:27Z jlconlin $
 $Revision: 284 $
 $Date: 2008-04-09 07:33:27 -0600 (Wed, 09 Apr 2008) $

 This is used to test the ArnoldiMatrix class
 */

#include<iostream>
#include "ArnoldiMatrixTest.h"
#include "ArnoldiMatrix.h"
#include "Utilities.h"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
typedef Teuchos::SerialDenseMatrix<int, double> dSDM;
typedef Teuchos::SerialDenseVector<int, double> dSDV;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;

using std::cout;
using std::endl;
using std::complex;

void BasicTest(dSDM, unsigned int);

int main( int argc, char* argv[] ){

    cout << "I'm testing ArnoldiMatrix class." << endl;

    unsigned int D = 5;

    // Define matrix elements
    Teuchos::SerialDenseMatrix<int, double> A(D, D);
    for( int i = 0; i < D; ++i ){
        A(i,i) = i + 1;
    }
    A(2,1) = 1.0;
    cout << "Matrix:\n" << A << endl;
     
    ArnoldiMatrixTest AMT(A);
//  AMT.Aq();

    BasicTest(A, D);
    return 0;
}


/*BasicTest merely runs Arnoldi's method on a (nearly) diagonal matrix*/
void BasicTest(dSDM A, unsigned int D){
      
    double Noise = 0.0;
    double R = 20;
    double I = 5;
    vector<unsigned long> seed(2);
    seed[0] = RandomLib::Random::SeedWord();
    seed[1] = 1;
    std::cout << "Master seed: " << seed[0] << "\n";

    // Starting vector
    Teuchos::SerialDenseVector<int, double> q(D);
    for( int i = 0; i < D; ++i ){
        q(i) = 1.0;
    }

    cout << "starting q:\n" << q << endl;
    // Restarted Arnoldi
    ArnoldiMatrix arnM(A, seed, Noise, true);
    arnM.arnoldi(q, I);
//  arnM.ERAM(q, R, I);
    cout << "\n\nEigenvalues:\n" << arnM.Values() << endl;
    cout << "Eigenvectors:\n" << arnM.Vectors() << endl;
    cout << "Q:\n" << arnM.Q() << endl;
    cout << "H:\n" << arnM.H() << endl;

}


