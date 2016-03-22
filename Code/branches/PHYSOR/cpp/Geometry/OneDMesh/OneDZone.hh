// Date Last Altered: $Date: 2007-01-06 16:37:40 -0500 (Sat, 06 Jan 2007) $
// Revision Number: $Revision: 71 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: OneDZone.hh
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides the interface for the one-dimensional Zone geometric sub-element.
///
/// This file provides the interface for the one-dimensional Zone class.  The Zone class
/// basically stores all of the information that you would expect a zone to contain (basically
/// just physical dimensions).  The zone has references to its left and right nodes as well.

#ifndef ONEDZONE_HH
#define ONEDZONE_HH

#include <ostream>
#include <limits>
#include <boost/compressed_pair.hpp>

using std::ostream;
using std::numeric_limits;
using boost::compressed_pair;

class Node;
class Corner;
class Zone;

#include "OneDNode.hh"
#include "OneDCorner.hh"
#include "GeomID.hh"

/*!  \addtogroup MeshSys Mesh Subsystem
 *  @{
*/
/// \brief The Zone class stores a zone's physical dimensions and references to its left and right nodes.
///
/// The Zone class contains all of the zone-specific information necessary to provide spatial
/// information to the various physics packages.  Specifically, it knows its spatial dimensions
/// and holds references to its neighboring Nodes.	
class Zone
{
public:
	/// Define the Id type as a ZoneId
	typedef ZoneId Id;
	
	// ****** Constructor ******
	/// Constructor for the Zone class
	Zone(double length_in, double area_in, unsigned int id_in)
			: mLength(length_in), mArea(area_in), mId(id_in), mNodes(0, 0),
			  mCorners(0, 0)
	{ }

	
	// ****** Data Accessors ******
	/// Returns the length of the Zone.
	double length() const { return mLength; }
	/// Returns the area of the Zone.
	double area() const { return mArea; }
	/// Returns the volume of the Zone.
	double volume() const { return mLength*mArea; }
	/// Returns the Zone::Id identification number.
	const Id& id() const { return mId; }
	/// Returns the coordinate of the Zone center.
	double center() const;
	
	// ****** Connectivity Accessors ******
	/// Returns a pair of pointers for both Nodes that the Zone is connected to.
	const compressed_pair<const Node*, const Node*>& getNodes() const { return mNodes; }
	
	/// Returns a reference to a particular connected Node.
	const Node& getNode(unsigned int index) const;
	/// Returns a reference to the left Node.
	const Node& getLeftNode() const { return *(mNodes.first()); }
	/// Returns a reference to the right Node.
	const Node& getRightNode() const { return *(mNodes.second()); }

	/// Returns a reference to a particular connected Corner.
	const Corner& getCorner(unsigned int index) const;
	/// Returns a reference to the left Corner.
	const Corner& getLeftCorner() const { return *(mCorners.first()); }
	/// Returns a reference to the right Corner.
	const Corner& getRightCorner() const { return *(mCorners.second()); }
	
	/// \brief Calculates the distance to the boundary of the Zone.
	double distanceToBndry(double x_pos, double angle) const;
	
private:
	friend class Mesh; ///< Mesh is a friend of Zone
	
	double mLength; ///< Length of the Zone.
	double mArea; ///< Length of the Node.
	Id mId; ///< Zone identification number.
	
	// Holds the Nodes that are connected to this Zone
	compressed_pair<const Node*, const Node*> mNodes; ///< Contains pointers to the connected Nodes.
	compressed_pair<const Corner*, const Corner*> mCorners; ///< Contains pointers to the connected Corners.
};

// Print out the zone id
/// This prints the Zone::Id Zone identification number to the output stream.
inline ostream& operator<<(ostream& out, const Zone::Id& id) { return out << id.id; }

///  @}

#endif
