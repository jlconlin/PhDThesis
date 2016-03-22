// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: GeomID.hh
/// \author Greg Davidson
/// \date November 5, 2006
///
/// \brief This file provides the GeomId classes.
///
/// This file provides the GeomID classes.  Every geometric element has a unique id number
/// which is stored in the appropriate id class.

#ifndef GEOMID_HH
#define GEOMID_HH

/// \defgroup MeshSys Mesh Subsystem
/*! \addtogroup MeshSys Mesh Subsystem
 *  The mesh subsystem consists of all of the classes and functionality necessary to
 *  fully describe and control the spatial problem domain.
 *  @{
 */

/// The ZoneId class holds the identification number for a zone.
class ZoneId
{
public:
	ZoneId(unsigned int id_in) : id(id_in) { }
	unsigned int id;
};

/// The NodeId class holds the identification number for a node.
class NodeId
{
public:
	NodeId(unsigned int id_in) : id(id_in) { }
	unsigned int id;
};

/// The CornerId class holds the identification number for a corner
class CornerId
{
public:
	CornerId(unsigned int id_in) : id(id_in) { }
	unsigned int id;
};

/// @}

#endif
