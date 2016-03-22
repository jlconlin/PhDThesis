// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: MeshTraits.hh
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides all of the mesh typedefs for the iterators.
///
/// This file provides all of the typedefs required by the Mesh class for the iterators and
/// groups them all into one place.  This helps to simplify the Mesh interface.

#ifndef MESHTRAITS_HH
#define MESHTRAITS_HH

#include "OneDZone.hh"
#include "OneDNode.hh"
#include "OneDCorner.hh"
#include "SweepIter.hh"
#include "Iterator.hh"
#include "ReverseIterator.hh"

/// \defgroup MeshSys Mesh Subsystem.
/*! \addtogroup MeshSys Mesh Subsystem
 *  @{
*/

/// \brief The MeshTraits class provides all of the type definitions for the iterators.
///
/// The MeshTraits class provides all of the type definitions needed by the mesh.  By
/// extracting these and placing them in a different class, we simplify the mesh interface.
class MeshTraits
{
public:
    // ****** Typedefs ******
	/// Iterator over the Zones.
    typedef Iterator< vector<Zone> > ZoneIterator;
    /// Constant iterator over the Zones.
    typedef Iterator< vector<Zone>, vector<Zone>::const_iterator> const_ZoneIterator;
	/// Iterator over the Nodes.
    typedef Iterator< vector<Node> > NodeIterator;
    /// Constant iterator over the nodes.
    typedef Iterator< vector<Node>, vector<Node>::const_iterator> const_NodeIterator;
	/// Iterator over the Corners.
	typedef Iterator< vector<Corner> > CornerIterator;
	/// Constant iterator over the Corners.
	typedef Iterator< vector<Corner>, vector<Corner>::const_iterator> const_CornerIterator;

	/// ReverseIterator over the Zones.
    typedef ReverseIterator< ZoneIterator > reverse_ZoneIterator;
	/// Constant ReverseIterator over the Zones.
    typedef ReverseIterator< const_ZoneIterator > const_reverse_ZoneIterator;
	/// ReverseIterator over the Nodes.
    typedef ReverseIterator< NodeIterator > reverse_NodeIterator;
	/// Constant ReverseIterator over the Nodes.
    typedef ReverseIterator< const_NodeIterator > const_reverse_NodeIterator;
	/// ReverseIterator over the Corners.
	typedef ReverseIterator< CornerIterator > reverse_CornerIterator;
	/// Constant ReverseIterator over the Corners.
	typedef ReverseIterator< const_CornerIterator > const_reverse_CornerIterator;
    
    /// Iterate over the zones in a sweep pattern.
    typedef SweepIter SweepIterator;
};

/// @}

#endif
