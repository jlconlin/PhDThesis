/*Material.i
 $Author: jlconlin $
 $Id: Material.i 220 2007-12-19 19:57:01Z jlconlin $
 $Revision: 220 $
 $Date: 2007-12-19 12:57:01 -0700 (Wed, 19 Dec 2007) $

 This is the SWIG interface file for the Material class.

 */
#ifndef MATERIAL_I
#define MATERIAL_I

%module Material

%{
#include "Material.h"
%}

%include "std_vector.i"
%include "std_string.i"

%feature("autodoc", "1");
%include "Material.h"

%extend Material {
    std::string __str__(){
        return $self->stringRep();
    }
};

/// Need this to instantiate a vector of Materials
%template(vectormats) std::vector<Material>;  

#endif

