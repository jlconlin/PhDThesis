// Date Last Altered: $Date: 2007-01-06 16:37:40 -0500 (Sat, 06 Jan 2007) $
// Revision Number: $Revision: 71 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: OneDCartMesh.cc
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides the implementation for the one-dimensional Cartesian Mesh class.
///
/// This file provides the implementation for the one-dimensional Cartesian Mesh class.  The
/// mesh keeps containers of all of the so-called geometric sub-elements, such as zones
/// and nodes.  Together with its sub-elements, the mesh contains all of the geometric
/// information necessary to fully describe a problem's spatial domain.

#include <numeric>
#include <limits>

#include "OneDCartMesh.hh"
#include "../Misc/Assert.hh"

using std::accumulate;
using std::numeric_limits;
using boost::addressof;
using boost::compressed_pair;


/// \brief This method actually does the work of allocating and connecting the
/// geometric sub-elements.
///
/// This is the workhorse method that actuall constructs and connects the geometric
/// sub-elements.  It consists of four parts.  First, it creates all of the zones.
/// Second, it creates all of the nodes.  Third, it connects the zones to their
/// appropriate node, and finally it connects the nodes to their appropriate zone.
/// The zones store references to their right and left nodes, and the nodes store
/// references to their right and left zones.  If a node has a null reference, it
/// knows it sits on the boundary of the spatial domain.
void Mesh::mBuildMesh(const vector<double>& zone_lengths)
{
	// Construct the Zones
	mZones.reserve(mNumZones);
	
	unsigned int zone_counter = 0;
	for(vector<double>::const_iterator iter = zone_lengths.begin();
		iter != zone_lengths.end(); ++zone_counter, ++iter)
	{
		mZones.push_back( Zone(*iter, mArea, zone_counter) );
	}
	
	// Construct the Nodes
	mNodes.reserve(mNumZones+1);
	
	unsigned int node_counter = 0;
	double length_counter = 0.0;
	for(vector<double>::const_iterator iter = zone_lengths.begin();
		iter != zone_lengths.end(); ++node_counter, ++iter)
	{
		mNodes.push_back( Node(length_counter, node_counter) );
		length_counter += *iter;
	}
	
	Assert( (length_counter - mLength) < 1E-10);
	// Build the last node
	mNodes.push_back( Node(length_counter, node_counter) );

	// Construct the Corners
	mCorners.reserve(mNumZones*2);
	
	for(unsigned int corner_counter = 0; corner_counter < mNumZones*2;
		++corner_counter)
	{
		mCorners.push_back( Corner(corner_counter) );
	}

	
	// Set the zone connectivity
	zone_counter = 0;
	for(vector<Zone>::iterator iter = mZones.begin(); iter != mZones.end();
		++zone_counter, ++iter)
	{
		unsigned int node_left = zone_counter;
		unsigned int node_right = zone_counter+1;

		unsigned int corner_left = zone_counter*2;
		unsigned int corner_right = corner_left+1;
		
		Assert(node_right < mNodes.size());
		iter->mNodes.first() = addressof(mNodes[node_left]);
		iter->mNodes.second() = addressof(mNodes[node_right]);

		Assert(corner_right < mCorners.size());
		iter->mCorners.first() = addressof(mCorners[corner_left]);
		iter->mCorners.second() = addressof(mCorners[corner_right]);
	}
	
	// Set the node connnectivity
	node_counter = 0;
	for(vector<Node>::iterator iter = mNodes.begin(); iter != mNodes.end();
		++node_counter, ++iter)
	{
		unsigned int zone_left = node_counter - 1;
		unsigned int zone_right = node_counter;

		unsigned int corner_left = zone_left*2 + 1;
		unsigned int corner_right = corner_left + 1;
		
		if(node_counter == 0) // First Node
		{
			Assert(zone_right < mZones.size() );
			iter->mZones.second() = addressof(mZones[zone_right]);

			Assert(corner_right < mCorners.size() );
			iter->mCorners.second() = addressof(mCorners[corner_right]);
		}
		else if(node_counter == mNodes.size()-1) // Last Node
		{
			Assert(zone_left < mZones.size() );
			iter->mZones.first() = addressof(mZones[zone_left]);

			Assert(corner_left < mCorners.size() );
			iter->mCorners.first() = addressof(mCorners[corner_left]);
		}
		else // Interior node
		{
			Assert(zone_left < mZones.size() );
			Assert(zone_right < mZones.size() );
			iter->mZones.first() = addressof(mZones[zone_left]);
			iter->mZones.second() = addressof(mZones[zone_right]);

			Assert(corner_left < mCorners.size() );
			Assert(corner_right < mCorners.size() );
			iter->mCorners.first() = addressof(mCorners[corner_left]);
			iter->mCorners.second() = addressof(mCorners[corner_right]);
		}
	}

	// Set the Corner connectivity
	unsigned int corner_counter = 0;
	for(vector<Corner>::iterator iter = mCorners.begin(); iter != mCorners.end();
		++corner_counter, ++iter)
	{
		unsigned int zone = corner_counter / 2;
		unsigned int node = corner_counter / 2 + corner_counter%2;

		Assert(zone < mZones.size());
		iter->mZone = addressof(mZones[zone]);
		iter->mNode = addressof(mNodes[node]);
	}
	
}

//Given a position, return the Zone in which that position is located
const Zone* Mesh::getZone(double x) const {
    //Ensure position is inside Mesh
    Assert( x >= 0 );
    Assert( x <= mLength );
    
    compressed_pair<const Node*, const Node*> zoneNodes;
    for( vector<Zone>::const_iterator iter = mZones.begin(); iter != mZones.end();
        ++iter ){
        zoneNodes = iter->getNodes();
        if( zoneNodes.first()->x() < x and zoneNodes.second()->x() >= x ){
            return &(*iter);
        }
    }
    return 0;
}