/*ArnoldiMatrixTest.h
 $Author: jlconlin $
 $Id: ArnoldiMatrixTest.h 284 2008-04-09 13:33:27Z jlconlin $
 $Revision: 284 $
 $Date: 2008-04-09 07:33:27 -0600 (Wed, 09 Apr 2008) $

 This class is used to test the ArnoldiMatrix class.  It is a friend class to 
 Arnoldi and therefore has access to all it's attributes.
 */

#ifndef ARNOLDIMATRIXTEST_H
#define ARNOLDIMATRIXTEST_H

#include<iostream>
#include "ArnoldiMatrix.h"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
typedef Teuchos::SerialDenseMatrix<int, double> dSDM;
typedef Teuchos::SerialDenseVector<int, double> dSDV;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;

class ArnoldiMatrixTest : public ArnoldiMatrix {
    public:
        ArnoldiMatrixTest(Teuchos::SerialDenseMatrix<int, double> A)
            : ArnoldiMatrix(A, vector<unsigned long>()) {  }

        void Aq();

    private:
        void _Init_Q(int, int);

};


void ArnoldiMatrixTest::Aq(){
    cout << "\n-----------------------------------------\n"
         << "I'm testing Aq()."
         << "\n-----------------------------------------" << endl;

    _k = 0;
    cSDV q(_A.numRows());
    _Init_Q(5,1);
    cout << "_Q:\n" << _Q << endl;

    q = _Aq();
    cout << "q = Aq:\n" << q << endl;


}


void ArnoldiMatrixTest::_Init_Q(int rows, int cols){
    _Q.shape(rows, cols);
    complex<double>* QIter = _Q.values();
    for( int i = 1; i < rows*cols + 1; ++i, ++QIter ){
        *QIter = complex<double>(i);
    }
}



#endif

