/*ArnoldiMatrix.cpp
 $Author: jlconlin $
 $Id: ArnoldiMatrix.cpp 282 2008-04-04 13:59:33Z jlconlin $
 $Revision: 282 $
 $Date: 2008-04-04 07:59:33 -0600 (Fri, 04 Apr 2008) $

 This is the implementation of Arnoldi's method for a smiple Matrix.
 */

#include <iostream>
#include <iomanip>
#include <ctime>
#include "ArnoldiMatrix.h"
#include "RandomLib/Random.hpp"
#include "RandomLib/NormalDistribution.hpp"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"

using std::setw;
using std::setprecision;
using std::fixed;
using std::scientific;
using std::cout;
using std::endl;

cSDV ArnoldiMatrix::_Aq(){
    
    cSDV q(Teuchos::Copy, _Q[_k], _Q.numRows());    //Make vector

    cSDV Return_q(_Q.numRows()); // Make vector for returning
    int success;
    success = Return_q.multiply(Teuchos::NO_TRANS, Teuchos::NO_TRANS, 1.0, _A, q, 0.0 );

    // Add noise
    if( _Noise > 0.0 ){
        RandomLib::NormalDistribution<double> N;
        cSDV NoiseVector(Return_q.length());
        complex<double>* Iter = NoiseVector.values();
        for( int i = 0; i < NoiseVector.length(); ++i, ++Iter ){
            *Iter = N(_r, 0, _Noise);
        }
        Return_q += NoiseVector;
    }
    return Return_q;
}


void ArnoldiMatrix::_Iteration(){
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    Arnoldi::_Iteration();
    _nowI = clock();
    if( _Verbose ){
        cout << "\tIteration #:" << setw(4) << _k+1
             << ", time:" << setprecision(5) << setw(8) << fixed
             << double(_nowI-_startI)/CLOCKS_PER_SEC << " sec." << fixed 
             << endl;
    }
}


void ArnoldiMatrix::_Restart(int I){
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
      
    // Restart vector is largest Ritzvector from previous iteration
    cSDV q = cSDV(Teuchos::Copy, &((_Ritzvectors[0])[0]), _length);

    Arnoldi::arnoldi(q, I);
    _nowR = clock();
    if( _Verbose ){
        cout << "Restart #:" << setw(4) << _restart
             << ", Eigvalue =" << setprecision(3) << setw(15) << _Ritzvalues[0]
             << ", residual =" << setprecision(2) << setw(9) << scientific
             << _Residual.back() << ", time:" << fixed << setw(6)
             << double(_nowR-_startR)/CLOCKS_PER_SEC << " sec."
             << endl;
    }
}


