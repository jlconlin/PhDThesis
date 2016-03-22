/*ArnoldiMatrix.i
 $Author: jlconlin $
 $Id: ArnoldiMatrix.i 259 2008-02-23 17:31:55Z jlconlin $
 $Revision: 259 $
 $Date: 2008-02-23 10:31:55 -0700 (Sat, 23 Feb 2008) $

 This is the SWIG interface file for the ArnoldiMatrix class.

 */
#ifndef ARNOLDIMATRIX_I
#define ARNOLDIMATRIX_I

%module ArnoldiMatrix

%{
#define SWIG_FILE_WITH_INIT
#include "ArnoldiMatrix.h"
#include "Arnoldi.h"
#include <iostream>
%}

%include "numpy.i"
%init %{ import_array(); %}
%feature("autodoc", "1");

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) Teuchos::SerialDenseMatrix<int, double>{
    $1 = PyArray_Check($input);
}

%typemap(in) Teuchos::SerialDenseMatrix<int, double> {
    if( PyArray_Check($input) ){
        PyArrayObject* tmp = (PyArrayObject* )$input;
        std::cout << "strides[0]:" << tmp->strides[0] 
                << ", strides[1]: " << tmp->strides[1] 
                << ", strides[2]: " << tmp->strides[2] << std::endl;
        $1 = Teuchos::SerialDenseMatrix<int, double>(Teuchos::Copy, 
            (double *)(tmp->data), (int)(tmp->strides[0]/sizeof(double)+1), 
            (int)(tmp->dimensions[0]), (int)(tmp->dimensions[1]));
    }
}


class ArnoldiMatrix{
    public:
        ArnoldiMatrix(Teuchos::SerialDenseMatrix<int, double> A, 
                bool v = true, double rT = 1E-10, double iT = 1E-12);

        cSDM A() const;
        void arnoldi(dSDV q, int I);

        double ResTolerance() const;
        double InvTolerance() const;
};

#endif

