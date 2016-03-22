// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file MeshExceptStrings.hh
 *  \author Greg Davidson
 *  \date May 13, 2008
 * 
 *  \brief Provides some templated output strings for use by the MeshExcept classes.
 * 
 *  This file provides some functions for printing out diagnostic strings for use by the
 *  MeshExcept exception classes. */
 
#ifndef MESHEXCEPTSTRINGS_HH
#define MESHEXCEPTSTRINGS_HH

#include <boost/lexical_cast.hpp>

#include "Dimension.hh"

using boost::lexical_cast;

/// Forward declaration of the \c Zone template class.
template<typename dimension_type> class Zone;
/// Forward declaration of the \c Node template class.
template<typename dimension_type> class Node;
/// Forward declaration of the \c Corner template class.
template<typename dimension_type> class Corner;

/*! \brief Returns the dimension string.
 *
 *  Returns a \c string holding the dimension string, which is the
 *  numerical spatial value plus a 'D', like "1D" or "2D".
 *  \return Returns a \c string holding the dimension string. */
template<typename dimension_type>
inline string dimensionString()
{
   function_requires< DimensionConcept<dimension_type> >();
   
   // For some reason, I have to save the dimension_type::value as a
   // variable or it won't link.  Not sure why... might be a bug in
   // the linker.   
   typename dimension_type::value_type val = dimension_type::value;
   return lexical_cast<string>(val) + 'D';
}

/*! \brief General geometric element string function.
 *
 *  This is a function that returns the geometric element string, i.e.
 *  (the name of the geometric function).  The general template is not
 *  implemented.  Template specializations must be used. 
 *  \par Template Parameters:
 *     <dl> <dt> \e geom_elem </dt>
 *          <dd> The geometric element whose name string we want. </dd> </dl> */
template<typename geom_element>
inline string geomElementString();

/*! \brief Returns the name of the \c Zone type.
 *
 *  Returns the \c string "Zone" plus the dimension string.
 *  \return A \c string holding the name of the \c Zone object. */
template<>
inline string geomElementString< Zone<OneD> >()
{ 
   return "Zone< " + dimensionString<OneD>() + " >";
}

/*! \brief Returns the name of the \c Node type.
 *
 *  Returns the \c string "Node" plus the dimension string.
 *  \return A \c string holding the name of the \c Node object. */
template<>
inline string geomElementString< Node<OneD> >()
{
   return "Node< " + dimensionString<OneD>() + " >";
}

/*! \brief Returns the name of the \c Corner type.
 *
 *  Returns the \c string "Corner" plus the dimension string.
 *  \return A \c string holding the name of the \c Corner object. */
template<>
inline string geomElementString< Corner<OneD> >()
{
   return "Corner< " + dimensionString<OneD>() + " >";
}


#endif

