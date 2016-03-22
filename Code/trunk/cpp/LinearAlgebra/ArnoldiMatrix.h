/*ArnoldiMatrix.h

 This is the header file for the ArnoldiMatrix class.  This class provides the
 implenation of Arnoldi's method for a simple Matrix.
*/

#ifndef ARNOLDIMATRIX_H
#define ARNOLDIMATRIX_H

#include<iostream>
#include <complex>
#include <vector>
#include "RandomLib/Random.hpp"
#include "RandomLib/NormalDistribution.hpp"
#include <limits>

template<typename T>
class Arnoldi;
#include "Arnoldi.h"

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/static_assert.hpp"

using std::complex;

namespace ublas =  boost::numeric::ublas;

class ArnoldiMatrix : public Arnoldi<double> {
    public:
        typedef ublas::matrix<double, ublas::column_major> Mat_t;

        ArnoldiMatrix(Mat_t Matrix, std::vector<unsigned long>& seed, int v = 1,
                double Tol = std::numeric_limits<double>::epsilon() )
            : Arnoldi<double>(v, Tol), _A(Matrix), _NormalD()
        { 
             _length = _A.size2();
            _r = new RandomLib::Random(seed);
        }

        void RAM(T_vec_t q, unsigned int n, unsigned int I, unsigned int R,
                RestartMethod M = IMPLICIT, double Noise = 0.0){
            assert( q.size() == _A.size2() );

            _Noise = Noise;
            Arnoldi<double>::RAM(q, n, I, R, M);
        }


        void arnoldi(T_vec_t q, unsigned int I){
            assert( q.size() == _A.size2() );

            Arnoldi<double>::arnoldi(q, I);
        }

        friend class ArnoldiMatrixTest;

    protected:
        Arnoldi<double>::T_vec_t _Av();
        void _Iteration(T_vec_t&);
        void _Restart(T_vec_t&);

    private:
        Mat_t _A;
        double _Noise;

        RandomLib::Random* _r;   //Random number generator
        RandomLib::NormalDistribution<double> _NormalD;

};
#endif

