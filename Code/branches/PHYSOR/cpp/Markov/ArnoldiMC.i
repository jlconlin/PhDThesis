/*ArnoldiMCBase.i
 $Author: jlconlin $
 $Id: ArnoldiMC.i 248 2008-01-18 14:18:25Z jlconlin $
 $Revision: 248 $
 $Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $

 This is the SWIG interface file for the Markov class.

 */
#ifndef ARNOLDIMCBASE_I
#define ARNOLDIMCBASE_I

%module ArnoldiMCBase

%{
#include "ArnoldiMCBase.h"
#include "Markov.h"
%}

%include "Misc.i"
%include "Field.i"

%feature("autodoc", "1");
// Necessary to avoid memory leaks, Markov_Transport returns allocated object
%newobject Markov::Markov_Transport;

%include "Markov.h"
%template(Markov_BankSource) Markov<BankSource>;
%template(Markov_HistSource) Markov<HistSource>;

%include "ArnoldiMCBase.h"
#endif

