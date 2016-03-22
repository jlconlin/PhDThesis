/*ArnoldiMatrix.h
 $Author: jlconlin $
 $Id: ArnoldiMatrix.h 342 2008-08-04 19:55:07Z jlconlin $
 $Revision: 342 $
 $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

 This is the header file for the ArnoldiMatrix class.  This class provides the
 implenation of Arnoldi's method for a simple Matrix.
 */

#ifndef ARNOLDIMATRIX_H
#define ARNOLDIMATRIX_H

#include<iostream>
#include <complex>
#include <limits>
class Arnoldi;
#include "Arnoldi.h"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/static_assert.hpp"

using std::complex;

namespace ublas =  boost::numeric::ublas;

class ArnoldiMatrix : public Arnoldi {
    public:
        typedef ublas::matrix<double, ublas::column_major> Mat_t;

        ArnoldiMatrix(Mat_t Matrix, bool v = true, 
                double Tol = std::numeric_limits<double>::epsilon() )
            : Arnoldi(v, Tol), _A(Matrix)
        { _length = _A.size2();   }

        void IRAM(dVec_t q, unsigned int n, unsigned int I, unsigned int R){
            assert( q.size() == _A.size2() );
            Arnoldi::IRAM(q, n, I, R);
        }

        void ERAM(dVec_t q, unsigned int n, unsigned int I, unsigned int R){
            assert( q.size() == _A.size2() );
            Arnoldi::ERAM(q, n, I, R);
        }

        void arnoldi(dVec_t q, unsigned int I){
            assert( q.size() == _A.size2() );

            Arnoldi::arnoldi(q, I);
        }

        friend class ArnoldiMatrixTest;

    protected:
        Arnoldi::cVec_t _Av();
        void _Iteration(cVec_t&);
        void _IRestart(cVec_t&);
        void _ERestart(cVec_t&);

    private:
        Mat_t _A;

};
#endif

