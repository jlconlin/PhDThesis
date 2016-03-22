/*ArnoldiMatrix.h
 $Author: jlconlin $
 $Id: ArnoldiMatrix.h 282 2008-04-04 13:59:33Z jlconlin $
 $Revision: 282 $
 $Date: 2008-04-04 07:59:33 -0600 (Fri, 04 Apr 2008) $

 This is the header file for the ArnoldiMatrix class.  This class provides the
 implenation of Arnoldi's method for a simple Matrix.
 */

#ifndef ARNOLDIMATRIX_H
#define ARNOLDIMATRIX_H

#include <complex>
class Arnoldi;
#include "Arnoldi.h"
#include "RandomLib/Random.hpp"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
typedef Teuchos::SerialDenseMatrix<int, double> dSDM;
typedef Teuchos::SerialDenseVector<int, double> dSDV;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;

using std::complex;

class ArnoldiMatrix : public Arnoldi {
    public:
        /*seed: Seed for random number generator
          Noise: Standard deviation of normal distribution of noise components
          v: Verbose true/false
          rT: Tolerance for Residual
          iT: Tolerance for Invariance*/
        ArnoldiMatrix(Teuchos::SerialDenseMatrix<int, double> A, 
                const std::vector<unsigned long>& seed, double Noise = 0.0,
                bool v = true, double rT = 1E-10, double iT = 1E-12)
            : Arnoldi(v, rT, iT), _A(A.numRows(), A.numCols()), _Noise(Noise),
              _r(seed){
                complex<double>* _AIter = _A.values();
                double* AIter = A.values();
                for( int i = 0; i < A.numRows()*A.numCols(); 
                        ++i, ++_AIter, ++AIter ){
                    *_AIter = *AIter;
                }
        }

        ArnoldiMatrix(Teuchos::SerialDenseMatrix<int, complex<double> > A, 
                const std::vector<unsigned long>& seed, double Noise = 0.0,
                bool v = true, double rT = 1E-10, double iT = 1E-12)
            : Arnoldi(v, rT, iT), _A(A), _Noise(Noise), _r(seed) {   }

        ~ArnoldiMatrix() {  }

        cSDM A() const { return _A; }

        friend class ArnoldiMatrixTest;

    private:
        cSDM _A;
        double _Noise;      // How much noise to add to a calculation

        RandomLib::Random _r;   //Random number generator

        void _Iteration();
        void _Restart(int);
        cSDV _Aq();
};
#endif

