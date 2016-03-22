/*FissionSource.i
 $Author: jlconlin $
 $Id: BankSource.i 239 2008-01-07 17:47:47Z jlconlin $
 $Revision: 239 $
 $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $

 This is the SWIG interface file for all the FissionSource classes.

 */
#ifndef BANKSOURCE_I
#define BANKSOURCE_I

%module BankSource

%{
#include "BankSource.h"
#include "FissionSource.h"
%}


%include "Field.i"
%include "Mesh.i"
%include "Misc.i"

%include "std_vector.i"
%feature("autodoc", "1");

//BankSource object
%include "FissionSource.h"
%rename(__len__) BankSource::size;
%include "BankSource.h"

%template(PointParticle) std::vector<Particle*>;
#endif
