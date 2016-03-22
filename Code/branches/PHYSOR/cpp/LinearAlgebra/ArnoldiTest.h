/*ArnoldiTest.h
 $Author: jlconlin $
 $Id: ArnoldiTest.h 282 2008-04-04 13:59:33Z jlconlin $
 $Revision: 282 $
 $Date: 2008-04-04 07:59:33 -0600 (Fri, 04 Apr 2008) $

 This class is used to test the Arnoldi class.  It is a friend class to Arnoldi
 and therefore has access to all it's attributes.
 */

#ifndef ARNOLDITEST_H
#define ARNOLDITEST_H

#include<iostream>
#include<complex>
#include<algorithm>
#include "Utilities.h"
#include "Arnoldi.h"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
typedef Teuchos::SerialDenseMatrix<int, double> dSDM;
typedef Teuchos::SerialDenseVector<int, double> dSDV;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;

using std::endl;
using std::cout;
using std::vector;
using std::complex;
using std::valarray;

class ArnoldiTest : public Arnoldi {
    public:
        ArnoldiTest(bool v = true, double rT = 1E-10, double iT = 1E-12)
            : Arnoldi(v, rT, iT) {  }

        void EigenSorting();
        void MakeRitzPairs();
        void Orthogonalize();
        void Normalize();

    private:
        Teuchos::SerialDenseVector<int, complex<double> > _values;
        Teuchos::SerialDenseMatrix<int, complex<double> > _vectors;

        void _InitValues();
        void _InitVect();
        void _Init_Q(int, int);
        void ArnoldiTest::_InitNorm_Q(int, int);

        // Need these to be defined
        void _Iteration() { }
        void _Restart(int) {  }
        cSDV _Aq() {    }

};

/*EigenSorting tests whether the functions that sort the Eigenpairs work.*/
void ArnoldiTest::EigenSorting(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing the EigenPair sorting functions." 
         << "\n-----------------------------------------" << endl;

    unsigned int N = 4;

    _InitValues();
    cout << "Eigenvalues:\n" << _values << endl;

    cout << "Initializing vectors..." << endl;
    _InitVect();
    cout << "Initialized vector:\n" << _vectors << endl;

    cout << "Making eigenpairs..." << endl;
    vector<Eigenpair<complex<double> > > EigenPairs(_values.length());
    _MakeEigenPair(_values, _vectors, EigenPairs);
    cout << "Eigenpairs:\n" << EigenPairs << endl;
    cout << "Addresses of vectors:" << endl;
    for( int i = 0; i < N; ++i ) cout << i+1 << ": " << _vectors[i] << ", ";

    std::sort(EigenPairs.begin(), EigenPairs.end());    // Sort Eigenpairs
    cout << "\nSorted Eigenpairs:\n" << EigenPairs << endl;

}


/*MakeRitzPairs tests _MakeRitzPairs function.  This is the tricky one.*/
void ArnoldiTest::MakeRitzPairs(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing MakeRitzPairs."
         << "\n-----------------------------------------" << endl;

    _length = 5;
    _k = 4;

    _InitValues();
    _InitVect();

    cout << "Eigenvectors:\n" << _vectors << endl;
    vector<Eigenpair<complex<double> > > EigenPairs(_values.length());
    _MakeEigenPair(_values, _vectors, EigenPairs);
    std::sort(EigenPairs.begin(), EigenPairs.end());    // Sort Eigenpairs
    cout << "Sorted Eigenpairs:\n" << EigenPairs << endl;
    vector<Eigenpair<complex<double> > >::iterator EPIter = EigenPairs.begin();
    for( ; EPIter != EigenPairs.end(); ++EPIter ){
        PrintVector(EPIter->Vector(), 4);
    }

    // Initialize _Q
    int rows = _length; int cols = EigenPairs.size() + 1;
    _Init_Q(rows, cols);
    cout << "\nQ:\n" << _Q << endl;

    _MakeRitzPairs(EigenPairs);
    cout << "\nRitzvalues:\n" << _Ritzvalues << endl;
    cout << "Ritzvectors:\n" << _Ritzvectors << endl;

}


void ArnoldiTest::Orthogonalize(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing Orthogonalize()."
         << "\n-----------------------------------------" << endl;

    _k = 2;
    _length = 4;
    _H.shape(_k+2,_k+1);
    _InitNorm_Q(_length, _k+1);
    cout << "_Q:\n" << _Q << endl;
    cSDV q(_length);
    for( int i = 0; i < _length; ++i ) q[i] = _length - i;
    cout << "q:\n" << q << endl;
    _Orthogonalize(q);
    cout << "Orthogonalized q:\n" << q << endl;
    cout << "_Q:\n" << _Q << endl;
    /*As near as I can tell the orthogonalizing function works.*/
}


void ArnoldiTest::Normalize(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing Normalize()."
         << "\n-----------------------------------------" << endl;

    _k = 1;
    _H.shape(_k+2,_k+1);
    cSDV q(5);
    for( int i = 0; i < 5; ++i ) q[i] = i+1;
    cout << "Un-normalized q:\n" << q << endl;
    _Normalize(q);
    cout << "Normalized q:\n" << q << endl;
    cout << "_H:\n" << _H << endl;
}


void ArnoldiTest::_InitValues(){
    unsigned int N = 4;
    _values.size(N);
    _values[0] = complex<double>(1,-1);
    _values[1] = complex<double>(-1,1);
    _values[2] = complex<double>(-1,-1);
    _values[3] = complex<double>(1,1);
}

void ArnoldiTest::_InitVect(){
    unsigned int N = 4;
    _vectors.shape(N,N);
    for( int i = 0; i < N; ++i ){
        for( int j = 0; j < N; ++j ){
            _vectors(i,j) = complex<double>(j+1,i+1);
        }
    }
}

void ArnoldiTest::_Init_Q(int rows, int cols){
    _Q.shape(rows, cols);
    complex<double>* QIter = _Q.values();
    for( int i = 1; i < rows*cols + 1; ++i, ++QIter ){
        *QIter = complex<double>(i);
    }
}


/*Initialize _Q with normalized columns*/
void ArnoldiTest::_InitNorm_Q(int rows, int cols){
    _Init_Q(rows, cols);
    for( int i = 0; i < cols; ++i ){
        cSDV q(Teuchos::View, _Q[i], rows);
        _Normalize(q);
    }
}


#endif

