/*PowerMCBase.i
 $Author: jlconlin $
 $Id: PowerMCBase.i 242 2008-01-08 19:08:09Z jlconlin $
 $Revision: 242 $
 $Date: 2008-01-08 12:08:09 -0700 (Tue, 08 Jan 2008) $

 This is the SWIG interface file for the Markov class.

 */
#ifndef POWERMCBASE_I
#define POWERMCBASE_I

%module PowerMCBase

%{
#include "PowerMCBase.h"
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
%template(Markov_HistSource) Markov<HistSource>;

%include "PowerMCBase.h"


#endif

