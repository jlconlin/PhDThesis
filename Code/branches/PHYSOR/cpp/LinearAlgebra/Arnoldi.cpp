/*Arnoldi.cpp
 $Author: jlconlin $
 $Id: Arnoldi.cpp 295 2008-05-05 17:23:46Z jlconlin $
 $Revision: 295 $
 $Date: 2008-05-05 11:23:46 -0600 (Mon, 05 May 2008) $

 The Arnoldi class is the C++ implementation of Arnoldi's method.
 */

#include <iostream>
#include <iomanip>
#include <complex>
#include <utility>
#include <algorithm>
#include <valarray>
#include <ctime>
#include "Utilities.h"
#include "Arnoldi.h"
#include "Teuchos_BLAS.hpp"
#include "Teuchos_LAPACK.hpp"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"


using std::setw;
using std::setprecision;
using std::fixed;
using std::scientific;
using std::cout;
using std::endl;
using std::vector;
using std::valarray;
using std::pair;
using std::complex;

/*Explicitily Restarted Arnoldi's Method
 * q: Starting vector
 * R: Number of Arnoldi Restarts
 * I: Number Arnoldi Iterations per restart*/
void Arnoldi::ERAM( cSDV s, int R, int I ){

    // Reset data
//  _Ritzvectors = vector<valarray<complex<double> > >();
//  _Ritzvalues = vector<complex<double> >();
    _Residual = vector<double>();

    _startR = clock();      // Start clock
    for( _restart = 0; _restart < R; ++_restart ){
        _Restart(I);
        if( _stopped ) break;
    }
}

/*arnoldi is the main method for this class.  
 * 
 * q: Starting vector
 * I: Number of Arnoldi Iterations*/
void Arnoldi::arnoldi(cSDV q, int I){
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    _stopped = false;
    _length = q.length();
    
    _H.shape(I+1,I);    // Initialize _H
    _Q.shape(q.length(), I+1);  // Initialize _Q
    q.scale(1.0/q.normFrobenius()); //Normalize
    for( int i = 0; i < q.length(); ++i ) _Q(i,0) = q(i);

    // Iterate
    _startI = clock();
    for( _k = 0; _k < I; ++_k ){
        _Iteration();

        // Determine if we should stop
        if( abs(_H(_k+1,_k)) < _InvTolerance ){
            _stopped = true;
            if( _Verbose ){
                cout << "Finished because _H[" << setw(3) << _k+1 << "," 
                    << setw(3) << _k << "] = " 
                    << setw(7) << setprecision(4) << scientific << _H(_k+1, _k)
                    << " < " << _InvTolerance 
                    << fixed << endl;
            }
            break;
        }
        else if( _Residual.back() < _ResTolerance ){
            _stopped = true;
            if( _Verbose ){
                cout << "Finished because Residual = "
                    << setw(7) << setprecision(4) << scientific 
                    << _Residual.back() << " < " << _ResTolerance 
                    << fixed << endl;
            }
            break;
        }
    }
}


void Arnoldi::_Iteration(){
    cSDV q = _Aq();      // Operate
    _Orthogonalize(q);
    _Normalize(q);
    _calcEigPairs(_k+1);

    // Add q to _Q
    complex<double>* iterQ = _Q[_k+1];
    complex<double>* iterq = q.values();
    for( int i = 0; i < _length; ++i, ++iterQ, ++iterq ) *iterQ = *iterq;
}


void Arnoldi::_Orthogonalize(cSDV& q){
    Teuchos::BLAS<int, complex<double> > blas;
    complex<double> OrthogFactor;

    for( int j = 0; j <= _k; ++j ){
        OrthogFactor = blas.DOT(_length, q.values(), 1, _Q[j], 1);
        _H[_k][j] = OrthogFactor;
        complex<double>* iterQ = _Q[j];
        complex<double>* iterq = q.values();
        for( int i = 0; i < _length; ++i, ++iterQ, ++iterq ){
           *iterq -= OrthogFactor*(*iterQ);
        }
    }
}


void Arnoldi::_Normalize(cSDV& q){
    complex<double> NormFactor = q.normFrobenius();
    _H(_k+1, _k) = NormFactor;
    q.scale(1.0/NormFactor);
}


/*_calcEigPairs will calculate the Ritz pairs of the linear operator by 
calculating the eigenpairs of _H.  It calls supporting functions for sorting
the eigenpairs and converting eigenpairs to Ritz pairs.

N: How many eigenpairs desired.*/
void Arnoldi::_calcEigPairs(int N){
    // Temporary storage
    Teuchos::SerialDenseVector<int, complex<double> > values(N);
    Teuchos::SerialDenseMatrix<int, complex<double> > vectors(N, N);

    //Extra Stuff---not exactly sure what they are all for
    Teuchos::SerialDenseMatrix<int, complex<double> > 
            M(Teuchos::Copy, _H.values(), _H.stride(), N, N);
    Teuchos::SerialDenseVector<int, complex<double> > Work(4*(N));
    Teuchos::SerialDenseVector<int, double> RWork(4*(N));
    int info;               //Exit flag

    {_lapack.GEEV(       // Extra brackets are for simple code folding
 /* IN */   'N',            /*Left eigenvectors ?
                              'N' --- do not calculate
                              'V' --- calculate*/
 /* IN */   'V',            /*Right eigenvectors ?
                              'N' --- do not calculate
                              'V' --- calculate*/
 /* IN */   N,              /*Order of matrix A. 
                              A.shape must be (NxN)*/
 /* IN */   M.values(),     /*Matrix
                              Must be 1D contiguous array in memory.
   OUT                        WARNING: Overwritten.*/
 /* IN */   M.stride(),     /*Leading dimension of A.
                              stride or number of columns of A.*/
 /* OUT*/   values.values(),  /*eigenvalues*/
 /* OUT*/   vectors.values(),     /*Left eigenvectors (if computed)
                                          not referenced if LDVL = 'N'*/
 /* IN */   1,              /*Leading dimension of Left eigenvectors
                              >=1,
                              >=N, if LDVL = 'V'.*/
 /* OUT*/   vectors.values(),     /*Right eigenvectors (if computed)
                                          not referenced if LDVR = 'N'*/
 /* IN */   N,           /*Leading dimension of Right eigenvectors
                              >=1,
                              >=N, if LDVR = 'V'.*/
 /* OUT*/   Work.values(),  /*Not sure what this does.*/
 /* IN */   4*(N),       /*Not sure what this does, complements Work.*/
 /* IN */   RWork.values(), /*Not sure what this does either.*/
 /* OUT*/   &info           /*Status of computation*/
            );}

    // Make Eigenpairs
    vector<Eigenpair<complex<double> > > EigenPairs(values.length());
    _MakeEigenPair(values, vectors, EigenPairs);
    std::sort(EigenPairs.begin(), EigenPairs.end());    // Sort Eigenpairs

    // Calculate Residual
    _calcResidual(EigenPairs);
    // Make Ritz pairs
    _MakeRitzPairs(EigenPairs);
}
    
/*_MakeEigenPair merely fills a vector of Eigenpairs wo they can be sorted.*/
void Arnoldi::_MakeEigenPair(cSDV& EigenValues, cSDM& EigenVectors,
        vector<Eigenpair<complex<double> > >& EigPairs){
    int i = 0;
    vector<Eigenpair<complex<double> > >::iterator epIter;
    for( epIter = EigPairs.begin(); epIter != EigPairs.end(); ++epIter, ++i ){
        *epIter = Eigenpair<complex<double> >(EigenValues[i], EigenVectors[i]);
    }
}


/*_calcResidual will calculate the residual norm and append it to the 
container for the Residual.
See Saad (1992) pp. 175-176.

SortedPairs: A list of sorted Eigenpairs of _H.*/
void Arnoldi::_calcResidual(vector<Eigenpair<complex<double> > >& SortedPairs){
    complex<double> VectorElement = *(SortedPairs.back().Vector()+_k);
    _Residual.push_back( std::abs( VectorElement*_H(_k+1,_k) ) );
}


void Arnoldi::_MakeRitzPairs(vector<Eigenpair<complex<double> > >& EigenPairs){

    int NumPairs = EigenPairs.size();
    _Ritzvalues = vector<complex<double> >(NumPairs);
    valarray<complex<double> > Rvector(_length);
    _Ritzvectors = vector<valarray<complex<double> > >(NumPairs, Rvector);

    int i = 0;

    // Create Arnoldi vectors
    vector< valarray<complex<double> > > ArnoldiVectors(_k+1,
            valarray<complex<double> >(_length));
    vector< valarray<complex<double> > >::iterator AVIter;
    for( AVIter = ArnoldiVectors.begin(); AVIter!= ArnoldiVectors.end();
            ++AVIter, ++i ){        // Iterate over all ArnoldiVectors to 
        // Make array from columns of _Q
        *AVIter = valarray<complex<double> >(_Q[i], _length);      
    }

    // Reversing order
    vector<Eigenpair<complex<double> > >::reverse_iterator epIter;
    vector< valarray<complex<double> > >::iterator RVIter = _Ritzvectors.begin();
    vector<complex<double> >::iterator RValIter = _Ritzvalues.begin();
    for( epIter = EigenPairs.rbegin(); epIter != EigenPairs.rend(); 
            ++epIter, ++RVIter, ++RValIter ){
        *RValIter = epIter->Value();    // Copy Ritz value

        // Make Ritz vector
        complex<double>* ei = epIter->Vector();
        AVIter = ArnoldiVectors.begin();
        // Iterate over all but last Arnoldi vector
        for( int i = 0; i < _k+1; ++i, ++AVIter, ++ei ){
            *RVIter += (*ei)*(*AVIter);
        }
    }
}

