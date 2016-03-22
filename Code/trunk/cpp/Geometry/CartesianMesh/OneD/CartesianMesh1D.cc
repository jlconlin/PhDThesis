// Date Last Altered: $Date: 2009-03-10 10:56:40 -0600 (Tue, 10 Mar 2009) $
// Revision Number: $Revision: 480 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file CartesianMesh1D.cc
 *  \author Greg Davidson
 *	 \date September 26, 2005
 *
 *  \brief Provides the implementation for the one-dimensional CartesianMesh class.
 *
 *  This file provides the implementation for the one-dimensional CartesianMesh class.  The
 *  mesh keeps containers of all of the so-called geometric sub-elements, such as Zones
 *  and Nodes.  Together with its sub-elements, the mesh contains all of the geometric
 *  information necessary to fully describe a problem's spatial domain. */

#include <algorithm>
#include <functional>
#include <iterator>

#include "CartesianMesh1D.hh"

using std::find_if;
using std::bind2nd;
using std::less_equal;
using std::distance;

/*! Constructor.  Takes in the mesh length and area, the number of \c Zone objects,
 *  the number of left \c GhostZone objects, and whether the left \c GhostZones mirror the
 *  left or right side of the mesh, the number of right \c GhostZone objects, and whether
 *  the right \c GhostZones mirror the left or right side of the mesh.  Also takes in the
 *  location of the left-most real \c Node of the mesh.
 *  \param[in] length_in              The length of the mesh.
 *  \param[in] area_in                The area of the mesh.
 *  \param[in] num_zones              The number of real \c Zone s in the mesh.
 *  \param[in] num_left_ghost_zones   The number of \c GhostZones on the left boundary.
 *  \param[in] ghost_left             Whether the left \c GhostZones mirror the left side of 
 *                                    the mesh.
 *  \param[in] num_right_ghost_zones  The number of \c GhostZones on the right boundary.
 *  \param[in] ghost_right            Whether the right \c GhostZones mirror the right side 
 *                                    of the mesh. 
 *  \param[in] left_node_location     The position where the "real" part of the mesh begins. */
CartesianMesh<OneD>::CartesianMesh(LengthType length_in, LengthType area_in, SizeType num_zones,
				                       SizeType num_left_ghost_zones, bool ghost_left,
				                       SizeType num_right_ghost_zones, bool ghost_right,
				                       Point<OneD> left_node_location)
		: CartesianMesh_base<OneD>( length_in, num_zones, num_zones+1, 2*num_zones,
		                            num_left_ghost_zones+num_right_ghost_zones,
		                            num_left_ghost_zones+num_right_ghost_zones,
		                            2*(num_left_ghost_zones+num_right_ghost_zones) ),
		  mArea(area_in),
		  mNumLeftGhostZones(num_left_ghost_zones),
		  mNumRightGhostZones(num_right_ghost_zones)
{
#ifndef NINPUTCHECK
   mDoCommonSenseChecks(num_zones);
#endif
	   
	vector<LengthType> zone_lengths(num_zones, length_in/num_zones);
	mCreateGeometricElements(zone_lengths, left_node_location);
	mConnectGeometricElements();
	
	// Create and connect the ghost zones if necessary
	if(num_left_ghost_zones != 0 || num_right_ghost_zones != 0)
	{
		mCreateGhostElements(ghost_left, ghost_right);
										  
		if(num_left_ghost_zones != 0) mConnectLeftGhostElements();
		if(num_right_ghost_zones != 0) mConnectRightGhostElements();
	}
}

/*! Constructor.  Takes in a \c vector of \c Zone lengths and area, 
 *  the number of left \c GhostZone objects, and whether the left \c GhostZones 
 *  mirror the left or right side of the mesh, the number of right \c GhostZone objects, 
 *  and whether the right \c GhostZones mirror the left or right side of the mesh.  
 *  Also takes in the location of the left-most real \c Node of the mesh.
 *  \param[in] zone_lengths           A \c vector of \c Zone lengths.
 *  \param[in] area_in                The area of the mesh.
 *  \param[in] num_left_ghost_zones   The number of \c GhostZones on the left boundary.
 *  \param[in] ghost_left             Whether the left \c GhostZones mirror the left side of 
 *                                    the mesh.
 *  \param[in] num_right_ghost_zones  The number of \c GhostZones on the right boundary.
 *  \param[in] ghost_right            Whether the right \c GhostZones mirror the right side 
 *                                    of the mesh. 
 *  \param[in] left_node_location     The position where the "real" part of the mesh begins. */
CartesianMesh<OneD>::CartesianMesh(const vector<LengthType>& zone_lengths, LengthType area_in,
						                 SizeType num_left_ghost_zones, bool ghost_left,
				                       SizeType num_right_ghost_zones, bool ghost_right,
				                       Point<OneD> left_node_location)
		: CartesianMesh_base<OneD>( accumulate(zone_lengths.begin(), zone_lengths.end(), 0.0),
		                            zone_lengths.size(), zone_lengths.size()+1,
		                            2*zone_lengths.size(),
		                            num_left_ghost_zones+num_right_ghost_zones,
		                            num_left_ghost_zones+num_right_ghost_zones,
		                            2*(num_left_ghost_zones+num_right_ghost_zones) ),
		   mArea(area_in),
		   mNumLeftGhostZones(num_left_ghost_zones),
		   mNumRightGhostZones(num_right_ghost_zones)
{
#ifndef NINPUTCHECK
   mDoCommonSenseChecks(zone_lengths);
#endif

   mCreateGeometricElements(zone_lengths, left_node_location);
	mConnectGeometricElements();
	
	if(num_left_ghost_zones != 0 || num_right_ghost_zones != 0)
	{
		mCreateGhostElements(ghost_left, ghost_right);
		                                 
		if(num_left_ghost_zones != 0) mConnectLeftGhostElements();
		if(num_right_ghost_zones != 0) mConnectRightGhostElements();
   }
}


/*! This function performs some common sense checks on the data given to the
 *  mesh constructor.  Throws exceptions if any of the data is found to be bad.
 *  \param[in] num_zones        The number of zones in the mesh.
 *  \throw BadCartMesh1DLength  Throws \c BadCartMesh1DLength if the length of the
 *                              mesh is negative.
 *  \throw BadCartMesh1DArea    Throws \c BadCartMesh1DArea if the area of the mesh
 *                              is negative.
 *  \throw MeshHasZeroZones     Throws \c MeshHasZeroZones if the number of <tt>Zone</tt>s 
 *                              in the mesh is zero. */
void CartesianMesh<OneD>::mDoCommonSenseChecks(SizeType num_zones)
                          throw(BadCartMesh1DLength, BadCartMesh1DArea, MeshHasZeroZones)
{
   if(Base::mLength <= 0.0) 
      throw BadCartMesh1DLength(__LINE__, __FILE__, mLength);
   if(mArea <= 0.0) 
      throw BadCartMesh1DArea(__LINE__, __FILE__, mArea);
   if(num_zones == 0) 
      throw MeshHasZeroZones(__LINE__, __FILE__);
}

/*! This function performs some common sense checks on the data given to the
 *  mesh constructor.                Throws exceptions if any of the data is found to be bad.
 *  \param[in] zone_lengths      A \c vector storing the lengths of the \c Zone objects
 *                               in the mesh.
 *  \throw BadCartMesh1DLength   Throws \c BadCartMesh1DLength if the length of the
 *                               mesh is negative.
 *  \throw BadCartMesh1DArea     Throws \c BadCartMesh1DArea if the area of the mesh
 *                               is negative.
 *  \throw MeshHasZeroZones      Throws \c MeshHasZeroZones if the number of <tt>Zone</tt>s 
 *                               in the mesh is zero. 
 *  \throw NegativeZone1DLength  Throws \c NegativeZone1DLength if any of the
 *                               \c Zone objects have a non-positive length.  */
void CartesianMesh<OneD>::mDoCommonSenseChecks(const vector<LengthType>& zone_lengths)
	                       throw(BadCartMesh1DLength, BadCartMesh1DArea, MeshHasZeroZones,
	                             NegativeZone1DLength)
{
   if(Base::mLength <= 0.0) 
      throw BadCartMesh1DLength(__LINE__, __FILE__, mLength);
   if(mArea <= 0.0) 
      throw BadCartMesh1DArea(__LINE__, __FILE__, mArea);
   if(zone_lengths.size() == 0) 
      throw MeshHasZeroZones(__LINE__, __FILE__);
      
   // Look for a negative zone length
   vector<LengthType>::const_iterator neg_zone_iter;
   if( (neg_zone_iter = find_if( zone_lengths.begin(), zone_lengths.end(), 
                                 bind2nd(less_equal<LengthType>(), 0.0) ) ) 
           != zone_lengths.end() )
   {
      throw NegativeZone1DLength(__LINE__, __FILE__, distance(neg_zone_iter, zone_lengths.begin()),
                                 *neg_zone_iter);
   }
}


/*! Creates the real geometric elements.
 *  \param[in] zone_lengths        A \c vector of \c Zone lengths.
 *  \param[in] left_node_location  The location of the left-most real \c Node. */ 
void CartesianMesh<OneD>::mCreateGeometricElements(const vector<Zone::LengthType>& zone_lengths,
                                                   const Point<OneD>& left_node_location)
{
	// ***** Construct the Zones *****
	SizeType zone_counter = 0;
	for(vector<Zone::LengthType>::const_iterator iter = zone_lengths.begin();
		 iter != zone_lengths.end(); ++zone_counter, ++iter)
	{
		mZones.push_back( new Zone(Zone::Id(zone_counter), *iter, mArea) );
	}
	
	// ***** Construct the Nodes *****
	SizeType node_counter = 0;
	Zone::LengthType length_counter = left_node_location.x();
	for(vector<Zone::LengthType>::const_iterator iter = zone_lengths.begin();
		 iter != zone_lengths.end(); ++node_counter, ++iter)
	{
		mNodes.push_back( new Node( Node::Id(node_counter), 
		                            Node::PointType(length_counter) ) );
		length_counter += *iter;
	}
//  Assert( std::fabs(length_counter - left_node_location.x() - Base::mLength) 
//                       < 1000.0*numeric_limits<Zone::LengthType>::epsilon() );
	// Build the last node
	mNodes.push_back( new Node( Node::Id(node_counter), 
	                            Node::PointType(length_counter) ) );

	// ***** Construct the Corners *****
	for(SizeType corner_counter = 0; corner_counter < 2*mZones.size();
		 ++corner_counter)
	{
		mCorners.push_back( new Corner(Corner::Id(corner_counter)) );
	}
}

/// Sets the connectivity of the real elements.
void CartesianMesh<OneD>::mConnectGeometricElements()
{
	typedef Zone::Id::IdType    ZoneIdType;
	typedef Node::Id::IdType    NodeIdType;
	typedef Corner::Id::IdType  CornerIdType;
	
	// Set the zone connectivity
	SizeType zone_counter = 0;
	for(ptr_vector<Zone>::iterator zone_iter = mZones.begin(); zone_iter != mZones.end();
		 ++zone_counter, ++zone_iter)
	{
		NodeIdType node_left  = zone_counter;
		NodeIdType node_right = zone_counter + 1;

		CornerIdType corner_left  = 2*zone_counter;
		CornerIdType corner_right = corner_left + 1;
		
		Zone& zone = *zone_iter;
		Assert(node_right < Base::mNodes.size());
		zone.mNodes.first = addressof( Base::mNodes[node_left] );
		zone.mNodes.second = addressof( Base::mNodes[node_right] );

		Assert(corner_right < mCorners.size());
		zone.mCorners.first = addressof( Base::mCorners[corner_left] );
		zone.mCorners.second = addressof( Base::mCorners[corner_right] );
	}
	
	// Set the node connnectivity
	SizeType node_counter = 0;
	for(ptr_vector<Node>::iterator node_iter = mNodes.begin(); node_iter != mNodes.end();
		 ++node_counter, ++node_iter)
	{
		ZoneIdType zone_left  = node_counter - 1;
		ZoneIdType zone_right = node_counter;

		CornerIdType corner_left  = 2*zone_left + 1;
		CornerIdType corner_right = corner_left + 1;
		
		Node& node = *node_iter;
		
		if(node_counter == 0) // First Node
		{
			Assert(zone_right < Base::mZones.size() );
			node.mZones.second   = addressof( Base::mZones[zone_right] );

			Assert(corner_right < Base::mCorners.size() );
			node.mCorners.second = addressof( Base::mCorners[corner_right] );
		}
		else if(node_counter == mNodes.size()-1) // Last Node
		{
			Assert(zone_left < Base::mZones.size() );
			node.mZones.first  = addressof( Base::mZones[zone_left] );

			Assert(corner_left < Base::mCorners.size() );
			node.mCorners.first = addressof( Base::mCorners[corner_left] );
		}
		else // Interior node
		{
			Assert(zone_left < Base::mZones.size() );
			Assert(zone_right < Base::mZones.size() );
			node.mZones.first = addressof( Base::mZones[zone_left] );
			node.mZones.second = addressof( Base::mZones[zone_right] );

			Assert(corner_left < Base::mCorners.size() );
			Assert(corner_right < Base::mCorners.size() );
			node.mCorners.first = addressof( Base::mCorners[corner_left] );
			node.mCorners.second = addressof( Base::mCorners[corner_right] );
		}
	}

	// Set the Corner connectivity
	SizeType corner_counter = 0;
	for(ptr_vector<Corner>::iterator corner_iter = Base::mCorners.begin(); 
	    corner_iter != mCorners.end(); ++corner_counter, ++corner_iter)
	{
		ZoneIdType zone = corner_counter / 2;
		NodeIdType node = corner_counter / 2 + corner_counter%2;

		Assert(zone < Base::mZones.size());
		Assert(node < Base::mNodes.size());
		corner_iter->mZone = addressof( Base::mZones[zone] );
		corner_iter->mNode = addressof( Base::mNodes[node] );
	}
}

/*! Creates the GhostElement objects.
 *  \param[in] ghost_left   Whether the left \c GhostZones mirror the left side of 
 *                          the mesh.
 *  \param[in] ghost_right  Whether the right \c GhostZones mirror the right side of
 *                          the mesh. */
void CartesianMesh<OneD>::mCreateGhostElements(bool ghost_left, bool ghost_right)
{
	// Do some simple parameter checks
	Assert(mNumLeftGhostZones + mNumRightGhostZones < Base::mZones.size());
	// Get some data
	SizeType num_zones   = Base::mZones.size();
	SizeType num_nodes   = Base::mNodes.size();
	SizeType num_corners = Base::mCorners.size();
	
	// Create the left ghost zones
	for(SizeType left_gz_count = 0; left_gz_count < mNumLeftGhostZones; ++left_gz_count)
	{
		if(ghost_left)
		{
		   Assert(left_gz_count < Base::mZones.size());
			Base::mGhostZones->push_back( new GhostZone( mZones[left_gz_count], 
			                                             GhostZone::Id(num_zones + left_gz_count) ) );
	   }
		else
		{
		   Assert(num_zones - 1 - left_gz_count < Base::mZones.size());
			Base::mGhostZones->push_back( new GhostZone( Base::mZones[num_zones - 1 - left_gz_count],
												                  GhostZone::Id(num_zones + left_gz_count) ) );
	   }
	}
	// Create the left ghost nodes
	LengthType x_pos_L = Base::mNodes.begin()->x();
	ptr_vector<GhostZone>::const_iterator gz_iter_L = Base::mGhostZones->begin();
	SizeType node_counter = 0;
	for(SizeType left_gn_count = 0; left_gn_count < mNumLeftGhostZones; ++left_gn_count)
	{
	   x_pos_L -= gz_iter_L->length();
		if(ghost_left)
		{
		   Assert( left_gn_count+1 < Base::mNodes.size() );
			Base::mGhostNodes->push_back( new GhostNode( Base::mNodes[left_gn_count+1], 
			                                             GhostNode::Id(num_nodes + left_gn_count) ) );
		}
		else
		{
		   Assert( num_nodes - 2 - left_gn_count < Base::mNodes.size() );
			Base::mGhostNodes->push_back( new GhostNode( Base::mNodes[num_nodes - 2 - left_gn_count], 
												                  GhostNode::Id(num_nodes + left_gn_count) ) );
		}
		
		Assert(node_counter < Base::mGhostNodes->size());
		(*Base::mGhostNodes)[node_counter].mPosition = Point<OneD>(x_pos_L);
		
		++gz_iter_L, ++node_counter;
	}
	// Create the left ghost corners
	for(SizeType left_gc_count = 0; left_gc_count < 2*mNumLeftGhostZones; ++left_gc_count)
	{
		if(ghost_left)
		{
		   Assert(left_gc_count < Base::mCorners.size());
			Base::mGhostCorners->push_back( new GhostCorner( Base::mCorners[left_gc_count], 
			                                                 GhostCorner::Id(num_corners + left_gc_count) ) );
	   }
		else
		{
		   Assert(num_corners - 1 - left_gc_count < Base::mCorners.size());
			Base::mGhostCorners->push_back( new GhostCorner( Base::mCorners[num_corners - 1 - left_gc_count],
													                   GhostCorner::Id(num_corners + left_gc_count) ) );
		}
	}

	// Create the right ghost zones
	for(SizeType right_gz_count = 0; right_gz_count < mNumRightGhostZones; ++right_gz_count)
	{
		if(ghost_right)
		{
		   Assert(num_zones-right_gz_count-1 < Base::mZones.size());
			Base::mGhostZones->push_back( new GhostZone( Base::mZones[num_zones - right_gz_count-1],
												                  GhostZone::Id(num_zones + mNumLeftGhostZones 
												                                    + right_gz_count) ) );
		}
		else
		{
		   Assert(right_gz_count < Base::mZones.size());
			Base::mGhostZones->push_back( new GhostZone( Base::mZones[right_gz_count],
												                  GhostZone::Id(num_zones + mNumLeftGhostZones 
												                                    + right_gz_count) ) );
		}
	}
	// Create the right ghost nodes
	LengthType x_pos_R = ( *(Base::mNodes.end()-1) ).x();
	ptr_vector<GhostZone>::const_iterator gz_iter_R = Base::mGhostZones->begin() + mNumLeftGhostZones;
	node_counter = mNumLeftGhostZones;
	for(SizeType right_gn_count = 0; right_gn_count < mNumRightGhostZones; ++right_gn_count)
	{
	   x_pos_R += gz_iter_R->length();
		if(ghost_right)
		{
		   Assert(num_nodes-1-right_gn_count < Base::mNodes.size());
			Base::mGhostNodes->push_back( new GhostNode( Base::mNodes[num_nodes - 1 - right_gn_count], 
  					         							         GhostNode::Id(num_nodes + mNumLeftGhostZones 
  					         							                                    + right_gn_count) ) );
  		}
		else
		{
		   Assert(right_gn_count+1 < Base::mNodes.size());
			Base::mGhostNodes->push_back( new GhostNode( Base::mNodes[right_gn_count + 1], 
												                  GhostNode::Id(num_nodes + mNumLeftGhostZones 
												                                             + right_gn_count) ) );
		}
		
		Assert( node_counter < Base::mGhostNodes->size() );
		(*Base::mGhostNodes)[node_counter].mPosition = Point<OneD>(x_pos_R);
		
		++gz_iter_R, ++node_counter;
	}
	// Create the right ghost corners
	for(SizeType right_gc_count = 0; right_gc_count < 2*mNumRightGhostZones; ++right_gc_count)
	{
		if(ghost_right)
		{
		   Assert(num_corners-right_gc_count-1 < Base::mCorners.size());
			Base::mGhostCorners->push_back( new GhostCorner( Base::mCorners[num_corners-right_gc_count-1],
													                   GhostCorner::Id(num_corners + 2*mNumLeftGhostZones 
													                                                   + right_gc_count) ) );
		}
		else
		{
		   Assert(right_gc_count < Base::mCorners.size());
			Base::mGhostCorners->push_back( new GhostCorner( Base::mCorners[right_gc_count],
													                   GhostCorner::Id(num_corners + 2*mNumLeftGhostZones 
													                                                   + right_gc_count) ) );
		}
	}
}

/// Sets the connectivity of the left ghost elements.
void CartesianMesh<OneD>::mConnectLeftGhostElements()
{
	Assert( Base::mGhostZones->size() == Base::mGhostNodes->size())
	Assert( Base::mGhostZones->size() == Base::mGhostCorners->size()/2 );	
	
	// **** Connect the left GhostZones ****
	// First left ghost zone
	(*Base::mGhostZones)[0].mNodes.first   = addressof( (*Base::mGhostNodes)[0] );
	(*Base::mGhostZones)[0].mNodes.second   = addressof( Base::mNodes[0] );
	(*Base::mGhostZones)[0].mCorners.first = addressof( (*Base::mGhostCorners)[0] );
	(*Base::mGhostZones)[0].mCorners.second = addressof( (*Base::mGhostCorners)[1] );
	// Remainder left ghost zones
	SizeType gz_counter = 1;
	ptr_vector<GhostNode>::const_iterator node_iter_Z     = Base::mGhostNodes->begin();
	ptr_vector<GhostCorner>::const_iterator corner_iter_Z = Base::mGhostCorners->begin()+2;
	ptr_vector<GhostZone>::iterator zone_iter_Z           = Base::mGhostZones->begin()+1;
	while(gz_counter < mNumLeftGhostZones)
	{
		zone_iter_Z->mNodes.first   = addressof( *(node_iter_Z+1) );
		zone_iter_Z->mNodes.second   = addressof( *node_iter_Z );
		zone_iter_Z->mCorners.first = addressof( *corner_iter_Z );
		zone_iter_Z->mCorners.second = addressof( *(++corner_iter_Z) );

		++gz_counter, ++node_iter_Z, ++corner_iter_Z, ++zone_iter_Z;
	}
	// **** Connect the left GhostNodes ****
	SizeType gn_counter = 0;
	ptr_vector<GhostZone>::const_iterator zone_iter_N     = Base::mGhostZones->begin();
	ptr_vector<GhostCorner>::const_iterator corner_iter_N = Base::mGhostCorners->begin();
	ptr_vector<GhostNode>::iterator node_iter_N           = Base::mGhostNodes->begin();
	while(gn_counter < mNumLeftGhostZones-1)
	{
	   Assert( zone_iter_N+1 != mGhostZones->end() );
		node_iter_N->mZones.first    = addressof( *(zone_iter_N+1) );
		node_iter_N->mZones.second    = addressof( *zone_iter_N );
		
		Assert( corner_iter_N+1 != mGhostCorners->end() );
	   node_iter_N->mCorners.first  = addressof( *(corner_iter_N+3) );
		node_iter_N->mCorners.second  = addressof( *corner_iter_N );

		
		++gn_counter, ++zone_iter_N, corner_iter_N+=2, ++node_iter_N;
	}
	// Do last left node
	node_iter_N->mZones.second   = addressof( *zone_iter_N );
	node_iter_N->mCorners.second = addressof( *corner_iter_N );
	// **** Connect the Ghost Corners ****
	// First corner
	ptr_vector<GhostCorner>::iterator corner_iter_C   = Base::mGhostCorners->begin();
	ptr_vector<GhostNode>::const_iterator node_iter_C = Base::mGhostNodes->begin();
	ptr_vector<GhostZone>::const_iterator zone_iter_C = Base::mGhostZones->begin();
	
	corner_iter_C->mZone = addressof( *zone_iter_C );
	corner_iter_C->mNode = addressof( *node_iter_C );
	++corner_iter_C;
	corner_iter_C->mZone = addressof( *zone_iter_C );
	corner_iter_C->mNode = addressof( *Base::mNodes.begin() );
	
	++corner_iter_C, ++node_iter_C, ++zone_iter_C;
	SizeType gc_counter = 1;
	while(gc_counter < mNumLeftGhostZones)
	{
	   corner_iter_C->mZone = addressof( *zone_iter_C );
	   corner_iter_C->mNode = addressof( *node_iter_C );
	   ++corner_iter_C;
	   corner_iter_C->mZone = addressof( *zone_iter_C );
	   corner_iter_C->mNode = addressof( *(node_iter_C-1) );
	
	   ++zone_iter_C, ++node_iter_C, ++corner_iter_C, ++gc_counter;
	}
	
	// ****** Connect the first node *******
	Base::mNodes.begin()->mZones.first   = addressof( *(Base::mGhostZones->begin()) );
	Base::mNodes.begin()->mCorners.first = addressof( *(Base::mGhostCorners->begin()+1) );
}

/// Sets the connectivity of the right ghost elements.
void CartesianMesh<OneD>::mConnectRightGhostElements()
{
	Assert( Base::mGhostZones->size() == Base::mGhostNodes->size());
	Assert( Base::mGhostZones->size() == Base::mGhostCorners->size()/2 );
	
	// **** Connect the right GhostZones ****
	// First right ghost zone
	(*Base::mGhostZones)[mNumLeftGhostZones].mNodes.first   = addressof( Base::mNodes[mNodes.size()-1] );
	(*Base::mGhostZones)[mNumLeftGhostZones].mNodes.second   
	                                 = addressof( (*Base::mGhostNodes)[mNumLeftGhostZones] );
	(*Base::mGhostZones)[mNumLeftGhostZones].mCorners.first 
	                                 = addressof( (*Base::mGhostCorners)[2*mNumLeftGhostZones] );
	(*Base::mGhostZones)[mNumLeftGhostZones].mCorners.second 
	                                 = addressof( (*Base::mGhostCorners)[2*mNumLeftGhostZones+1] );
	// Remainder right ghost zones
	SizeType gz_counter = 1;
	ptr_vector<GhostNode>::const_iterator node_iter_Z     = Base::mGhostNodes->begin() + mNumLeftGhostZones;
	ptr_vector<GhostCorner>::const_iterator corner_iter_Z = Base::mGhostCorners->begin() + 2*mNumLeftGhostZones + 2;
	ptr_vector<GhostZone>::iterator zone_iter_Z           = Base::mGhostZones->begin() + mNumLeftGhostZones + 1;
	while(gz_counter < mNumRightGhostZones)
	{
		zone_iter_Z->mNodes.first   = addressof( *node_iter_Z );
		zone_iter_Z->mNodes.second   = addressof( *(node_iter_Z+1) );
		zone_iter_Z->mCorners.first = addressof( *corner_iter_Z );
		zone_iter_Z->mCorners.second = addressof( *(++corner_iter_Z) );

		++gz_counter, ++node_iter_Z, ++corner_iter_Z, ++zone_iter_Z;
	}
	// **** Connect the right GhostNodes ****
	SizeType gn_counter = 0;
	ptr_vector<GhostZone>::const_iterator zone_iter_N     = Base::mGhostZones->begin() + mNumLeftGhostZones;
	ptr_vector<GhostCorner>::const_iterator corner_iter_N = Base::mGhostCorners->begin() + 2*mNumLeftGhostZones + 1;
	ptr_vector<GhostNode>::iterator node_iter_N           = Base::mGhostNodes->begin() + mNumLeftGhostZones;
	while(gn_counter < mNumRightGhostZones-1)
	{
	   Assert(zone_iter_N+1 != Base::mGhostZones->end());
		node_iter_N->mZones.first    = addressof( *zone_iter_N );
		node_iter_N->mZones.second    = addressof( *(zone_iter_N+1) );
		
		Assert(corner_iter_N+1 != Base::mGhostCorners->end());
		node_iter_N->mCorners.first  = addressof( *corner_iter_N );
		node_iter_N->mCorners.second  = addressof( *(++corner_iter_N) );
		
		++gn_counter, ++zone_iter_N, ++corner_iter_N, ++node_iter_N;
	}
	// Do last left node
	node_iter_N->mZones.first   = addressof( *zone_iter_N );
	node_iter_N->mCorners.first = addressof( *corner_iter_N );
	// **** Connect the Ghost Corners ****
	// First corner
	ptr_vector<GhostCorner>::iterator corner_iter  = Base::mGhostCorners->begin() + 2*mNumLeftGhostZones;
	ptr_vector<GhostNode>::const_iterator left_node_iter = Base::mGhostNodes->begin() + mNumLeftGhostZones;
	ptr_vector<GhostNode>::const_iterator right_node_iter = Base::mGhostNodes->begin() + mNumLeftGhostZones;
	ptr_vector<GhostZone>::const_iterator zone_iter    = Base::mGhostZones->begin() + mNumLeftGhostZones;
	
	corner_iter->mZone = addressof( *zone_iter );
	corner_iter->mNode = addressof( *(mNodes.end()-1) );
	++corner_iter;
	corner_iter->mZone = addressof( *zone_iter );
	corner_iter->mNode = addressof( *right_node_iter );
	
	++corner_iter, ++right_node_iter, ++zone_iter;
	SizeType gc_counter = 1;
	while(gc_counter < mNumRightGhostZones)
	{
	   corner_iter->mZone = addressof( *zone_iter );
	   corner_iter->mNode = addressof( *left_node_iter );
	   ++corner_iter;
	   corner_iter->mZone = addressof( *zone_iter );
	   corner_iter->mNode = addressof( *right_node_iter );
	
	   ++zone_iter, ++left_node_iter, ++right_node_iter, ++corner_iter, ++gc_counter;
	}
	
	// ****** Connect the last node *******
	(mNodes.end()-1)->mZones.second   = addressof( *(Base::mGhostZones->begin() + mNumLeftGhostZones) );
	(mNodes.end()-1)->mCorners.second = addressof( *(Base::mGhostCorners->begin() + 2*mNumLeftGhostZones) );
}											  
	
