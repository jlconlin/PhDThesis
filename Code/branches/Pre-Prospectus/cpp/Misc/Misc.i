//--------------------------------------------*-SWIG-*------------------------------------//
/// \file Misc.i
/// \author $Author: jlconlin $
/// \date $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $
/// $Id: Misc.i 239 2008-01-07 17:47:47Z jlconlin $
///
/// \brief This file contains group-wide traits for all SWIG files.
///

#ifndef MISC_I
#define MISC_I

%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"
%init %{ import_array(); %}

%include "exception.i"
%include "std_vector.i"

/// Exception handler to convert C++ STL exceptions to Python
%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

/// Define the templated versions of several vectors
%template(DVector) std::vector<double>;
%template(ULongVector) std::vector<unsigned long>;

%typemap(out) std::vector<double> {
    int length = $1.size();
    $result = PyArray_FromDims(1, &length, PyArray_DOUBLE); 
    memcpy(PyArray_DATA($result),&((*(&$1))[0]),sizeof(double)*length);
}
#endif 
