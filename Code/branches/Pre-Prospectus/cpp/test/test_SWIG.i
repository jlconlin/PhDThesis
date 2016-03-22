/*test_SWIG.i
 $Author: jlconlin $
 $Id: test_SWIG.i 239 2008-01-07 17:47:47Z jlconlin $
 $Revision: 239 $
 $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $

 This is the SWIG interface file for testing and learning SWIG features.

 */
#ifndef TEST_SWIG_I
#define TEST_SWIG_I

%module test_SWIG

%{
#include "Python.h"
#include "test_SWIG.h"
%}


%include "Misc.i"
%include "std_vector.i"
%feature("autodoc", "1");

%include "test_SWIG.h"

#endif

