/*Arnoldi.h
 $Author: jlconlin $
 $Id: Arnoldi.h 397 2008-10-13 21:06:07Z jlconlin $
 $Revision: 397 $
 $Date: 2008-10-13 15:06:07 -0600 (Mon, 13 Oct 2008) $

 The Arnoldi class is the C++ implementation of Arnoldi's method.  It is the 
 base class from which other Arnoldi's method implentations will inherit.
 */

#ifndef ARNOLDI_H
#define ARNOLDI_H

#include<iostream>
#include<complex>
#include<vector>
#include<valarray>
#include "Teuchos_LAPACK.hpp"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
#include "Eigenpair.h"
#include "Utilities.h"

using std::complex;
using std::vector;
using std::valarray;

typedef Teuchos::SerialDenseVector<int, double> dSDV;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;

class Arnoldi {
    public:

        /*Explicitily Restarted Arnoldi's Method*/
        void ERAM( cSDV q, int R, int I );

        /*ERAM converts the starting vector from a double array to a complex 
          array
            q: Starting vector
            R: Number of Arnoldi Restarts
            I: Number of Iterations per Restart*/
        void ERAM( dSDV s, int R, int I ){
            cSDV q = DtoCArray(s);
            ERAM(q, R, I);
        }

        /*Main method for this class.  This performs Arnoldi's method for 
         * starting vector q, and I iterations.*/
        void arnoldi(cSDV q, int I);
        void arnoldi(dSDV s, int I){
            cSDV q = DtoCArray(s);
            arnoldi(q, I);
        }

        // Gettr methods
        vector<complex<double> > Values() const
            { return _Ritzvalues; }
        vector<valarray<complex<double> > > Vectors() const
            { return _Ritzvectors; }
        Teuchos::SerialDenseMatrix<int, complex<double> > H() const
            { return _H; }
        Teuchos::SerialDenseMatrix<int, complex<double> > Q() const
            { return _Q; }
        double ResTolerance() const { return _ResTolerance; }
        double InvTolerance() const { return _InvTolerance; }
        vector<double> Residual() const { return _Residual; }

        friend class ArnoldiTest;

    protected:
        /* Making this the only contructor requies derived classes pass these 
           values and prevents direct construction of this class.*/
        Arnoldi( bool v, double rT, double iT )
            : _startI(), _nowI(), _startR(), _nowR(), _stopped(false), _k(0),
            _Q(), _Ritzvalues(), _Ritzvectors(), _H(), _Residual(),
            _Verbose(v), _ResTolerance(rT), _InvTolerance(iT), 
            _length(0), _lapack()
            { }

        virtual ~Arnoldi() {    }

        // Members used by derived classes.
        clock_t _startI; // For Iteration timing
        clock_t _nowI;   // For Iteration timing
        clock_t _startR; // For Restart timing
        clock_t _nowR;   // For Restart timing
        bool _stopped;  // To determine if we have stopped due to invariance
        int _restart;  // Restart number
        unsigned int _k;    // Iteration number
        int _length;    // Length of vectors, number of rows of _Q
        bool _Verbose;
        vector<double> _Residual;
        Teuchos::SerialDenseMatrix<int, complex<double> > _Q; // Arnoldi vectors
        Teuchos::SerialDenseMatrix<int, complex<double> > _H; // Upper Hessenberg matrix
    

        vector<valarray<complex<double> > > _Ritzvectors;
        vector<complex<double> > _Ritzvalues;

        /*Methods required by derived classes.*/
        virtual void _Iteration() = 0;
        virtual void _Restart(int) = 0;
        virtual cSDV _Aq() = 0;
        
        /*DtoCArray converts a double array, d, to a complex<double> array.*/
        cSDV DtoCArray(dSDV d){
            // Convert data to complex
            cSDV q(d.length());
            double*  dIter = d.values();
            complex<double>* qIter = q.values();
            // Copy data to complex vector
            for( int i = 0; i < d.length(); ++i, ++dIter, ++qIter ){
                *qIter = *dIter;
            }
            return q;
        }

    private:
                double _ResTolerance;    // Tolerance for Residual
        double _InvTolerance;    // Tolerance for Invariance

        Teuchos::LAPACK<int, complex<double> > _lapack;

        //Private methods
        void _Orthogonalize(cSDV& q);
        void _Normalize(cSDV& q);
        void _calcEigPairs(int);
        void _MakeEigenPair(cSDV&, cSDM&,
                vector<Eigenpair<complex<double> > >&);
        void _calcResidual(vector<Eigenpair<complex<double> > >&);
        void _MakeRitzPairs(vector<Eigenpair<complex<double> > >&);

};

#endif

