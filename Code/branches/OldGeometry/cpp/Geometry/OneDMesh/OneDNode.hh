// Date Last Altered: $Date: 2007-01-06 16:37:40 -0500 (Sat, 06 Jan 2007) $
// Revision Number: $Revision: 71 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: OneDNode.hh
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides the interface for the one-dimensional Node geometric sub-element.
///
/// This file provides the interface for the one-dimensional Node class.  The Node class
/// stores positional information and references to its connected Zones.

#ifndef ONEDNODE_HH
#define ONEDNODE_HH

#include <ostream>
#include <boost/compressed_pair.hpp>

using boost::compressed_pair;
using std::ostream;

class Zone;
class Corner;
class Node;

#include "OneDZone.hh"
#include "OneDCorner.hh"
#include "MeshExcept.hh"
#include "GeomID.hh"

/*!  \addtogroup MeshSys Mesh Subsystem
 *   @{
*/

/// \brief The Node class stores positional information and zone connectivities.
///
/// The Node class stores the position of the node, and also stores pointers to the zones that
/// the Node is connected to (on its left and right).  A Node is on the domain boundary if it
/// does not contain both a left and a right Zone reference.
class Node
{
public:
	/// Define the Id type as z NodeId
	typedef NodeId Id;
		
	// ****** Constructor ******
	/// Constructor for the Node class
	Node(const double x_pos, unsigned int id_in)
			: mXPos(x_pos), mId(id_in), mZones(0,0), mCorners(0,0)
	{ }
	
	// ****** Data Accessors ******
	/// Returns the x-position of the node.
	double x() const { return mXPos; }
	/// Returns the Node::Id identification number.
	const Id& id() const { return mId; }
	
	
	// ****** Connectivity Accessors ******
	/// Returns a pair of pointers for both Zones that the Node is connected to.
	const compressed_pair<const Zone*, const Zone*>& getZones() const { return mZones; }
	/// \brief Returns a reference to a particular Zone.  Throws NoZoneConnected if the
	/// reference is null.
	const Zone& getZone(unsigned int index) const throw(NoZoneConnected);
	/// Returns a reference to the left Zone.  Throws NoZoneConnected if the reference is null.
	const Zone& getLeftZone() const throw(NoZoneConnected) 
	{ 
		if(mZones.first()==0) throw NoZoneConnected(__LINE__, __FILE__);
	  	return *(mZones.first());  
	}
	/// Returns a reference to the right Zone.  Throws NoZoneConnected if the reference is null.	
	const Zone& getRightZone() const throw(NoZoneConnected) 
	{ 
		if(mZones.second()==0) throw NoZoneConnected(__LINE__, __FILE__);
		return *(mZones.second()); 
	}

	/// Returns a pair of pointers for both Corners that the Node is connected to.
	const compressed_pair<const Corner*, const Corner*>& getCorners() const { return mCorners; }
	/// \brief Returns a reference to a particular Corner.  Throws NoCornerConnected if the
	/// reference is null.
	const Corner& getCorner(unsigned int index) const throw(NoCornerConnected);
	/// Returns a reference to the left Corner. Throws NoCornerConnected if the reference is null.
	const Corner& getLeftCorner() const throw(NoCornerConnected)
	{
		if(mCorners.first()==0) throw NoCornerConnected(__LINE__, __FILE__);
		return *(mCorners.first());
	}
	/// Returns a reference to the right Corner.  Throws NoCornerConnected if the reference is null.
	const Corner& getRightCorner() const throw(NoCornerConnected)
	{
		if(mCorners.second()==0) throw NoCornerConnected(__LINE__, __FILE__);
		return *(mCorners.second());
	}
	
	/// Returns true if the node is on the domain boundary and returns false otherwise.
	bool onBoundary() const 
	{ 
		if(onLeftBoundary() || onRightBoundary()) return true;
		else return false;
	}
	/// Returns true if the node is on the left boundary and returns false otherwise
	bool onLeftBoundary() const
	{
		if(mZones.first() == 0) return true;
		else return false;
	}
	///. Returns true if the node is on the right boundary and returns false otherwise
	bool onRightBoundary() const
	{
		if(mZones.second() == 0) return true;
		else return false;
	}
	
private:
	/// Define the Id type as a NodeId
	friend class Mesh;
	
	double mXPos; ///< Spatial position of the Node.
	Id mId; ///< Node identification number.

	// Holds the Zones that are connected to this Node
	compressed_pair<const Zone*, const Zone*> mZones;  ///< Contains pointers to the connected Zones.
	compressed_pair<const Corner*, const Corner*> mCorners; ///< Contains pointers to the connected Corners.
};

// Print out the Node id
/// This prints the None::Id Node identification number to the output stream.
inline ostream& operator<<(ostream& out, const Node::Id& id) { return out << id.id; }

///  @}

#endif
