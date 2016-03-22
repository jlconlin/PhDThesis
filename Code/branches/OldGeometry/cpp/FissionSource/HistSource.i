/*HistSource.i
 $Author: jlconlin $
 $Id: HistSource.i 246 2008-01-09 21:46:05Z jlconlin $
 $Revision: 246 $
 $Date: 2008-01-09 14:46:05 -0700 (Wed, 09 Jan 2008) $

 This is the SWIG interface file for the HistSource class.

 */
#ifndef HISTSOURCE_I
#define HISTSOURCE_I

%module HistSource

%{
#include "HistSource.h"
#include "Field.hh"
%}

%include "Field.i"
%include "Mesh.i"
%include "Misc.i"


//HistSource object
%include "FissionSource.h"
%feature("autodoc", "1");
%include "HistSource.h"

#endif

