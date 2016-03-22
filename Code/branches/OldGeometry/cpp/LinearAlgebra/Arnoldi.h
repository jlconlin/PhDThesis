/*Arnoldi.h
 $Author: jlconlin $
 $Id: Arnoldi.h 342 2008-08-04 19:55:07Z jlconlin $
 $Revision: 342 $
 $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

 The Arnoldi class is the C++ implementation of Arnoldi's method.  It is the 
 base class from which other Arnoldi's method implentations will inherit.
 */

#ifndef ARNOLDI_H
#define ARNOLDI_H

#include<iostream>
#include<complex>
#include<vector>
#include<utility>
#include<limits>
#include "boost/compressed_pair.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_vector.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/microsec_time_clock.hpp"

using std::complex;
using std::cout;
using std::endl;

using boost::posix_time::ptime;
namespace ublas = boost::numeric::ublas;

class Arnoldi {
    public:
        typedef ublas::vector<double> dVec_t;
        typedef ublas::vector<complex<double> > cVec_t;
        typedef ublas::matrix<double, ublas::column_major> dMat_t;
        typedef ublas::matrix<complex<double>, ublas::column_major> cMat_t;
        typedef ublas::matrix_column<dMat_t> dColumn_t;
        typedef ublas::matrix_column<cMat_t> cColumn_t;
        typedef cMat_t::size_type cSize_t;
        typedef std::pair<complex<double>, unsigned int> ePair;

        /*arnoldi is the main method for the class.  It performs Arnoldi's 
         * method for starting vector q, and I iterations.*/
        void arnoldi(dVec_t& q, unsigned int I){
            cVec_t v(q); arnoldi(v, I);}
        void arnoldi(cVec_t& q, unsigned int I);

        /*IRAM is Implicitly Restarted Arnoldi's Method.*/
        void IRAM(dVec_t q, unsigned int n, unsigned int I, unsigned int R);

        /*ERAM is Explicitly Restarted Arnoldi's Method.*/
        void ERAM(dVec_t q, unsigned int n, unsigned int I, unsigned int R);

        const cMat_t& H() const { return _H; }
        const cMat_t& Q() const { return _V; }
        const cVec_t& Eigenvalues() const { return _Eigenvalues; }
        const cVec_t& Ritzvalues() const { return _Eigenvalues; }
        const cMat_t& Eigenvectors() const { return _Eigenvectors; }
        const cMat_t& Ritzvectors() const { return _Ritzvectors; }

        bool ePairComp(ePair&, ePair&);

        friend class ArnoldiTest;

    protected:
        /*Tolerance is the the smallest possible value.*/
        Arnoldi(bool v = true, 
                double Tol = std::numeric_limits<double>::epsilon() )
            : _I(0), _n(0), _k(0), _H(), _V(), _Verbose(v), _Tolerance(Tol), 
              _stoppedI(false), _stoppedR(false), _start(), _startR(),
              _Eigenvalues(), _Eigenvectors(), _Ritzvectors(), _residual(0.0)
        {   }

        unsigned int _I;    // Total number of iterations
        unsigned int _n;    // Number of desired eigenvalues
        unsigned int _k;    // Current iteration number
        unsigned int _R;    // Current restart number
        cMat_t _H;
        cMat_t _V;
        cVec_t _Eigenvalues;
        cMat_t _Eigenvectors;
        cMat_t _Ritzvectors;
        complex<double> _residual;

        // Functions
        virtual cVec_t _Av() = 0;
        virtual void _Iteration(cVec_t&);
        virtual void _IRestart(cVec_t&);    // Implicit Restart
        virtual void _ERestart(cVec_t&);    // Explicit Restart
        void _CalcEigenPairs();
        void _MakeRitzPairs();

        // Supporting variables
        bool _Verbose;
        double _Tolerance;
        unsigned int _length;
        bool _stoppedI;     // Stop iterating
        bool _stoppedR;     // Stop restarting
        ptime _start;    // Start of Arnoldi
        ptime _startR;   // Start of a restart

    private:
        void _Orthogonalize(cVec_t&);
        void _Normalize(cVec_t&);
        void _SortEigenPairs(cVec_t&, cMat_t&);

        Arnoldi(Arnoldi&) { }

};


#endif

