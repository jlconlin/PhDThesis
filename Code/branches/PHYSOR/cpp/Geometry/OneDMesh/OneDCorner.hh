// Date Last Altered: $Date: 2007-01-06 16:37:40 -0500 (Sat, 06 Jan 2007) $
// Revision Number: $Revision: 71 $
//----------------------------------*-C++-*------------------------------------//
/// \file OneDCorner.hh
/// \author Greg Davidson
/// \date August 22, 2006
///
/// \brief This file provides the interface for the one-dimensional Corner geometric sub-element.
///
/// This file provides the interface for the one-dimensional Corner class.  The Corner class
/// does not provide any geometric information, but does provide a centering-type for the Field
/// class.  A Corner possesses references to a Zone and a Node.

#ifndef ONEDCORNER_HH
#define ONEDCORNER_HH

#include <ostream>
#include <limits>
using std::ostream;
using std::numeric_limits;

class Zone;
class Node;
class Corner;

#include "OneDZone.hh"
#include "OneDNode.hh"
#include "GeomID.hh"

/*!  \addtogroup MeshSys Mesh Subsystem
 *   @{
 */

/// \brief The Corner class provides a field-centering geometric element.
///
/// The Corner class does not store any information about the problem domain, but provides a new
/// centering for use with the Field class.  Typically, Corners are imagined to sit at the location
/// of a Node, but just inside the Zone.  Therefore, a one-dimensional Zone will have two Corners.
class Corner
{
public:
	/// Define the Id type as a CornerId
	typedef CornerId Id;

	// ****** Constructor *******
	/// Default Constructor
	Corner()
			: mId(numeric_limits<unsigned int>::max()), mZone(0), mNode(0)
	{ }
	
	// ****** Constructor *******
	/// Constructor for the Corner class
	explicit Corner(unsigned int id_in)
			: mId(id_in), mZone(0), mNode(0)
	{ }

	// ****** Copy Contructor ******
	/// Copy constructor for the corner class (needed since we have pointer data members)
	Corner(const Corner& corner_in)
			: mId(corner_in.mId), mZone(corner_in.mZone), mNode(corner_in.mNode)
	{ }
	
	// ****** Copy Operator ******
	/// Copy operator for corner class
	const Corner& operator=(const Corner& corner_in)
	{
		mId = corner_in.mId;
		mZone = corner_in.mZone;
		mNode = corner_in.mNode;

		return *this;
	}
	
	// ****** Data Accessors ******
	/// Returns the Corner::Id identification number.
	const Id& id() const { return mId; }


	// ****** Connectivity Accessors ******
	/// Returns a reference to the connected Zone.
	const Zone& getZone() const
	{
		return *mZone;
	}
	/// Returns a reference to the connected Node.
	const Node& getNode() const
	{
		return *mNode;
	}

private:
	friend class Mesh; ///< Mesh is a friend of Corner
	
	Id mId; ///< Corner identification number.

	const Zone* mZone; ///< Reference to the connected Zone.
	const Node* mNode; ///< Reference to the connected Node.
};

// Print out the Corner id
/// This prints the Corner::Id Corner identification number to the output stream.
inline ostream& operator<<(ostream& out, const Corner::Id& id) { return out << id.id; }

///  @}

#endif
