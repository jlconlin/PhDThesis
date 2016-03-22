// Date Last Altered: $Date: 2007-01-06 16:37:40 -0500 (Sat, 06 Jan 2007) $
// Revision Number: $Revision: 71 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: OneDCartMesh.hh
/// \author Greg Davidson
/// \date June 3, 2005
///
/// \brief This file provides the interface for the one-dimensional Cartesian Mesh class.
///
/// This file provides the interface for the one-dimensional Cartesian Mesh class.  The
/// mesh keeps containers of all of the so-called geometric sub-elements, such as zones
/// and nodes.  Together with its sub-elements, the mesh contains all of the geometric
/// information necessary to fully describe a problem's spatial domain.

#ifndef ONEDCARTMESH_HH
#define ONEDCARTMESH_HH

#include <vector>
#include <numeric>
#include <boost/utility.hpp>

class Zone;
class Node;
class Corner;

#include "OneDZone.hh"
#include "OneDNode.hh"
#include "OneDCorner.hh"
#include "MeshTraits.hh"
#include "Assert.hh"

using std::vector;
using std::accumulate;

/// \defgroup MeshSys Mesh Subsystem.
/*! \addtogroup MeshSys Mesh Subsystem
 *  The mesh subsystem consists of all of the classes and functionality necessary to
 *  fully describe and control the spatial problem domain.
 *  @{
 */

/// \brief The Mesh class is responsible for storing and controlling a problem's spatial 
/// domain information.
///
/// The Mesh class is responsible for storing and controlling a problem's spatial 
/// domain information.  First, the mesh stores the global length and area of the domain.
/// Second, the Mesh also stores the geometric elements, which are (for a one-dimensional
/// mesh), Zones, Nodes, and Corners.  Together with these geometric elements, the Mesh
/// stores all of the information necessary to fully describe a problem's spatial domain.
class Mesh : public MeshTraits, boost::noncopyable
{
public:
	// ******  Mesh Constructors  ******
    /// \brief Constructor to create a uniform one-dimensional Cartesian Mesh with \a num_zones
	/// zones.
	///
	/// This constructor creates a uniform (all Zones are of equal length), one-dimensional
	/// Cartesian Mesh.
	/// \param length    Length of the spatial domain.
	/// \param area      Cross-sectional area of the spatial domain.
	/// \param num_zones The number of zones to break the spatial domain into.
    Mesh(const double length_in, const double area_in, const unsigned int num_zones)
			: mLength(length_in),
			  mArea(area_in),
			  mNumZones(num_zones),
			  mZones(), mNodes(), mCorners()
	{
		vector<double> zone_lengths(num_zones, length_in/num_zones);
		mBuildMesh(zone_lengths);
	}

	/// Constructor to create a one-dimensional Cartesian mesh with variable zone lengths.
	///
	/// This constructor takes a vector of zone lengths and constructs a mesh from this.  It
	/// is not necessary that the zones all be the same length, so this constructor can be
	/// used to create non-uniform one-dimensional Cartesian meshes.
    explicit Mesh(const vector<double>& zone_lengths, const double area_in=1.0)
			: mLength( accumulate(zone_lengths.begin(), zone_lengths.end(), 0.0) ), 
			  mArea(area_in), mNumZones(zone_lengths.size()),
			  mZones(), mNodes(), mCorners()
	{
		mBuildMesh(zone_lengths);
	}
	
    // ****** Property Accessors ******
    /// Return the length of the problem domain.
    double length() const { return mLength; }
    /// Return the cross-sectional area of the problem domain.
    double area() const { return mArea; }
    /// Return the volume of the problem domain.
    double volume() const { return mLength*mArea; }
    /// Return the number of zones in the problem domain.
    unsigned int numZones() const { return mNumZones; }
    /// Return the number of nodes in the problem domain.
    unsigned int numNodes() const { return mNumZones+1; }
	/// Return the number of corners in the problem domain.
	unsigned int numCorners() const { return mNumZones*2; }
	
    // ****** Connectivity Accessors ******
    /// Given a zone id, return the associated zone.
    const Zone& getZone(const Zone::Id& id) const
	{
		Assert(id.id < mZones.size());
		return mZones[id.id];
	}
    /// Given a node id, return the associated node.
    const Node& getNode(const Node::Id& id) const
	{
		Assert(id.id < mNodes.size());
		return mNodes[id.id];
	}
	/// Given a Corner::Id, return the associated Corner.
	const Corner& getCorner(const Corner::Id& id) const
	{
		Assert(id.id < mCorners.size());
		return mCorners[id.id];
	}
    
    //Given a position, return the Zone in which that position is located
    const Zone* getZone(double) const;
	
    /// Given a zone id, return the associated zone.
    const Zone& operator[](const Zone::Id& id) const { return getZone(id); }
    /// Given a node id, return the associated node.
    const Node& operator[](const Node::Id& id) const { return getNode(id); }
	/// Given a Corner::Id, return the associated Corner.
	const Corner& operator[](const Corner::Id& id) const { return getCorner(id); }
	
    // ****** Zone Forward Iterator Access ******
    /// Return an iterator to the beginning of the zones
    ZoneIterator zoneBegin() { return ZoneIterator(mZones, mZones.begin()); }
    /// Return an iterator to the end of the zones
    ZoneIterator zoneEnd() { return ZoneIterator(mZones, mZones.end()); }
    /// Return a constant iterator to the beginning of the zones
    const_ZoneIterator zoneBegin() const { return const_ZoneIterator(mZones, mZones.begin()); }
    /// Return a constant iterator to the end of the zones
    const_ZoneIterator zoneEnd() const { return const_ZoneIterator(mZones, mZones.end()); }
	
    // ****** Node Forward Iterator Access ******
    /// Return an iterator to the beginning of the nodes
    NodeIterator nodeBegin() { return NodeIterator(mNodes, mNodes.begin()); }
    /// Return an iterator to the end of the nodes	
    NodeIterator nodeEnd() { return NodeIterator(mNodes, mNodes.end()); }
    /// Return a constant iterator to the beginning of the nodes	
    const_NodeIterator nodeBegin() const { return const_NodeIterator(mNodes, mNodes.begin()); }
    /// Return a constant iterator to the end of the nodes	
    const_NodeIterator nodeEnd() const { return const_NodeIterator(mNodes, mNodes.end()); }

	// ****** Corner Forward Iterator Access ******
	/// Return an iterator to the beginning of the corners
	CornerIterator cornerBegin() { return CornerIterator(mCorners, mCorners.begin()); }
	/// Return an iterator to the end of the Corners
	CornerIterator CornerEnd() { return CornerIterator(mCorners, mCorners.end()); }
	/// Return a constant iterator to the beginning of the Corners
	const_CornerIterator cornerBegin() const { return const_CornerIterator(mCorners, mCorners.begin()); }
	/// Return a constant iterator to the end of the Corners
	const_CornerIterator cornerEnd() const { return const_CornerIterator(mCorners, mCorners.end()); }
	
    // ****** Zone Reverse Iterator Access ******
    /// Return a reverse iterator to the reverse beginning of the zones
    reverse_ZoneIterator zoneRBegin() { return reverse_ZoneIterator(ZoneIterator(mZones, mZones.end())); }
    /// Return a reverse iterator to the reverse ending of the zones	
    reverse_ZoneIterator zoneREnd() { return reverse_ZoneIterator(ZoneIterator(mZones, mZones.begin())); }
    /// Return a constant reverse iterator to the reverse beginning of the zones	
    const_reverse_ZoneIterator zoneRBegin() const { return const_reverse_ZoneIterator(const_ZoneIterator(mZones, mZones.end())); }
    /// Return a constant reverse iterator to the reverse ending of the zones		
    const_reverse_ZoneIterator zoneREnd() const { return const_reverse_ZoneIterator(const_ZoneIterator(mZones, mZones.begin())); }
	
    // ****** Node Reverse Iterator Access ******
    /// Return a reverse iterator to the reverse beginning of the nodes
    reverse_NodeIterator nodeRBegin() { return reverse_NodeIterator(NodeIterator(mNodes, mNodes.end())); }
    /// Return a reverse iterator to the reverse ending of the nodes
    reverse_NodeIterator nodeREnd() { return reverse_NodeIterator(NodeIterator(mNodes, mNodes.begin())); }
    /// Return a constant reverse iterator to the reverse beginning of the nodes
    const_reverse_NodeIterator nodeRBegin() const { return const_reverse_NodeIterator(const_NodeIterator(mNodes, mNodes.end())); }
    /// Return a constant reverse iterator to the reverse ending of the nodes
    const_reverse_NodeIterator nodeREnd() const { return const_reverse_NodeIterator(const_NodeIterator(mNodes, mNodes.begin())); }

	// ****** Corner Reverse Iterator Access ******
	/// Return a reverse iterator to the reverse beginning of the Corners
	reverse_CornerIterator cornerRBegin() { return reverse_CornerIterator(CornerIterator(mCorners, mCorners.end()) ); }
	/// Return a reverse iterator to the reverse ending of the Corners
	reverse_CornerIterator cornerREnd() { return reverse_CornerIterator(CornerIterator(mCorners, mCorners.begin()) ); }
	/// Return a constant reverse iterator to the reverse beginning of the Corners
	const_reverse_CornerIterator cornerRBegin() const { return const_reverse_CornerIterator(const_CornerIterator(mCorners, mCorners.end()) ); }
	/// Return a constant reverse iterator to the reverse ending of the Corners
	const_reverse_CornerIterator cornerREnd() const { return const_reverse_CornerIterator(const_CornerIterator(mCorners, mCorners.begin()) ); }
	
    // ****** Sweep Iterator Access ******
    /// Return the beginning of the sweep
    SweepIterator sweepBegin(double mu) const
    {
		if(mu>0) return SweepIterator(mu, mZones, mZones.begin());
		else return SweepIterator(mu, mZones, mZones.end()-1);
    }
    /// Return the end of the sweep
    SweepIterator sweepEnd(double mu) const
    {
		if(mu>0) return SweepIterator(mu, mZones, mZones.end());
		else return SweepIterator(mu, mZones, mZones.begin()-1);
    }

private:
    // ****** Build the mesh ******
    /// This method creates the sub-elements and sets their connectivity.
    void mBuildMesh(const vector<double>& zone_lengths);

    // ****** Private Mesh Data ******
    const double mLength; ///< Length of the mesh.
    const double mArea; ///< Cross-sectional area of the mesh.
    const unsigned int mNumZones; ///< Number of zones in the mesh
	
    vector<Zone> mZones; ///< Vector of zones.
    vector<Node> mNodes; ///< Vector of nodes.
	vector<Corner> mCorners; ///< Vector Corners
};

// ****** Functions to get the number of geometric elements in the zone ******
/// \brief The default template specialization for this function
///
/// The numCentering function returns the number of sub-geometric
/// elements in the mesh, where the particular sub-geometric element
/// requested is given as the template parameter.  This function is
/// the default template specialization, which merely throws an
/// exception.
template<typename GeomElem>
inline unsigned int numCentering(const Mesh& mesh);

/// \brief The Zone template specialization for this function.
///
/// This numCentering function specialization is specialized on
/// Zone.  It returns the number of zones in the mesh.
template<>
inline unsigned int numCentering<Zone>(const Mesh& mesh)
{
    return mesh.numZones();
}

/// \brief The Node template specialization for this function.
///
/// This numCentering function specialization is specialized on
/// Node.  It returns the number of Nodes in the Mesh.
template<>
inline unsigned int numCentering<Node>(const Mesh& mesh)
{
    return mesh.numNodes();
}

/// \brief The Corner template specialization for this function.
///
/// This numCentering function specialization is specialized on
/// Corner.  It returns the number of Corners in the Mesh.
template<>
inline unsigned int numCentering<Corner>(const Mesh& mesh)
{
	return mesh.numCorners();
}

/// @}
#endif


