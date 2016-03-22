/*PowerBase.i
 $Author: jlconlin $
 $Id: PowerBase.i 227 2007-12-21 21:25:11Z jlconlin $
 $Revision: 227 $
 $Date: 2007-12-21 14:25:11 -0700 (Fri, 21 Dec 2007) $

 This is the SWIG interface file for the Markov class.

 */
#ifndef POWERBASE_I
#define POWERBASE_I

%module PowerBase

%{
#include "PowerBase.h"
#include "Markov.h"
%}

%include "Field.i"
%include "Material.i"
%include "Misc.i"

%feature("autodoc", "1");
// Necessary to avoid memory leaks, Markov_Transport returns allocated object
%newobject Markov::Markov_Transport;
%include "Markov.h"
%template(Markov_BankSource) Markov<BankSource>;
%include "PowerBase.h"


#endif

