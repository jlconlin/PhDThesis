//----------------------* SWIG *------------------//
/// \file Field.i
/// \author $Author: jlconlin $
/// \date $Date: 2007-12-21 13:36:24 -0700 (Fri, 21 Dec 2007) $
/// $Id: Field.i 226 2007-12-21 20:36:24Z jlconlin $
///
/// \brief This file provides the python interface for the Field templated class.
///
/// This file provides the python interface for the Field templated class. 
/// Since a scripting language like python doesn''t understand templates, 
/// functionality is currently only defined for zone-centered fields of
/// doubles or Materials, and for node-centered fields of doubles.
/// Pythonic iteration is supported in a read-only way. 
/// 
/// Example: 
/// for item in myfield:
///     print item  
///     

#ifndef FIELD_I
#define FIELD_I

%module Field

%{
#include "Field.hh"
#include "OneDCartMesh.hh"
#include "OneDNode.hh"
#include "OneDZone.hh"
//#include "GeomIterator.hh"
#include "Iterator.hh"
#include "MeshTraits.hh"
#include "Material.h"
%}

/// These interfaces will be available from Field
%include "Mesh.i"
%include "Material.i"
%include "std_vector.i"
%include "Misc.i"

%feature("autodoc", "1");
/// This exposes Field to python
template<typename GeomElem, typename DataType>
class Field
{
public:
    typedef DataType value_type;
 /// Constructor using default constructor of \a value_type.
    explicit Field(const Mesh& mesh);

    /// Constructor using \a vector<DataType> \a data to initialize the field.
    Field(const Mesh& mesh, const std::vector<DataType>& data);

    /// Copy constructor
    Field(const Field<GeomElem, DataType>& field_in);

    /// Returns a constant reference to the data corresponding to element \a elem.
    //const_reference operator[](const GeomElem& elem) const;
    %extend{    // Extend Field object in Python to get indexing
        DataType __getitem__( const GeomElem& elem ) {
            return $self->operator[](elem);
        }

        void __setitem__(const GeomElem& elem, DataType value){
            $self->operator[](elem) = value;
        }
    }

    /// Returns the size of the Field (number of zones or nodes)
    %rename(__len__) size();
    const unsigned int size() { return mData.size(); }
};

/// Tell Python all the variants Field might be
%template(FieldZoneDouble) Field<Zone,double>;
%template(FieldNodeDouble) Field<Node,double>;
%template(FieldZoneMat) Field<Zone,Material>;

#if 0

/// Next we define the python class Iterator.
/// This exception catch is a little hack to tell all flavors of Iterator.next() 
/// when to raise StopIteration. We must define it before defining Iterator::next. 
%exception Iterator::next
{ 
    try { $action }
    catch ( IteratorOutOfRange )
    { 
        //PyErr_SetString(PyExc_StopIteration,"StopIteration called");  // either one works
        SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void());
        return NULL;
    }
}

/// Define the Field Iterator class from Greg and extend it to include the two 
/// necessary pythonic functions "next()" and "__iter__()".
template<typename Cont>
class Iterator 
{
public:
    Iterator();  // use default constructor from Field.hh
  
    // could expose other methods from here later...
 
    %extend
    {
        Iterator&  __iter__()
        {
            return( *($self) );
        }
    }
};


/// Tell Python all the variants iterator might be, and define
/// specific return functions for them (unfortunately) (since next() returns different types):
/// Note that we can extend a class outside of it's interface definition. 
%template(IteratorDouble) Iterator< std::vector<double> >;   
%extend Iterator< std::vector<double> >
{
    double next()
    {
        double& tmp_res = *(*($self));   // I don't understand this, but it works.
        ++(*($self));  // Call the increment operator Greg wrote
        return( tmp_res);  
    }

    double value()
    {
        return(*(*($self)));
    }
};

/// Iterator::next() for Materials
%template(IteratorMat) Iterator< std::vector<Material> >; 
%extend Iterator< std::vector<Material> >
{
    Material& next()
    {
        Material& tmp_res = *(*($self));
        ++(*($self));
        return( tmp_res);  
    }

    Material& value()
    {
        return(*(*($self)));
    }
};

#endif

#endif
