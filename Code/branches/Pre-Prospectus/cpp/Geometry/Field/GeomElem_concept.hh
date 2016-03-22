// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: GeomElem_concept.hh
/// \author Greg Davidson
/// \date November 6, 2006
/// 
/// \brief This file provides the GeomElem_concept class
///
/// This file provides the GeomElem_concept class.  This is provided so that only
/// geometric elements can be provided as template arguments to the field class.

#ifndef GEOMELEM_CONCEPT_HH
#define GEOMELEM_CONCEPT_HH

#include "OneDCartMesh.hh"

/// \defgroup FieldSys Field Subsystem
/*! \addtogruop FieldSys Field Subsystem
 *  The field subsystem consists of all of the classes and functionality necessary to
 *  hold geometry-dependent information on the spatial problem domain.
 *  @{
 */

/// \brief The GeomElem_concept class forces the user to only use geometric elements in fields.
///
/// The purpose of the GeomElem_concept class makes it impossible to make fields with a
/// centering that is not a geometric element.  If a class that is not a geometric element
/// is used as a template argument into a field, the code will not compile.
template<typename GeomElem>
struct GeomElem_concept
{
	void constraints()
	{
		numCentering<GeomElem>(m);
	}
	Mesh m;
};

/// @}
#endif
