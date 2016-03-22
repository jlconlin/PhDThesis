// Date Last Altered: $Date: 2008-11-17 13:16:10 -0700 (Mon, 17 Nov 2008) $
// Revision Number: $Revision: 419 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file CartesianMesh1D.hh
 *  \author Greg Davidson
 *	 \date June 3, 2005
 *
 *  \brief Provides the interface for the one-dimensional Cartesian mesh class.
 *
 *  This file provides the interface for the one-dimensional Cartesian mesh class.  The
 *  mesh keeps containers of all of the geometric elements, such as Zones
 *  and Nodes.  Together with its elements, the mesh contains all of the geometric
 *  information necessary to fully describe a problem's spatial domain. */

#ifndef CARTESIANMESH_HH
#define CARTESIANMESH_HH

#include <numeric>
#include <vector>

#include <boost/concept_check.hpp>
#include <boost/utility.hpp>

#include "Dimension.hh"
#include "CartesianMesh_base.hh"
#include "GeometricElementConcept.hh"

bool ZoneComp( const Zone<OneD>&, double );

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/// Forward declaration of the CartesianMesh class.
template<typename dimension_type> class CartesianMesh;
/// Forward declaration of the Zone class.
template<typename dimension_type> class Zone;
/// Forward declaration of the Node class.
template<typename dimension_type> class Node;
/// Forward declaration of the Corner class.
template<typename dimension_type> class Corner;

#include "Assert.hh"
#include "Types.hh"
#include "GeomId.hh"
#include "CartesianMesh_base.hh"
#include "Zone1D.hh"
#include "Node1D.hh"
#include "Corner.hh"
#include "SweepIterator1D.hh"
#include "GhostElement.hh"

using std::accumulate;
using std::vector;


/*! \brief The CartesianMesh class is responsible for storing and controlling a problem's 
 *         spatial domain information.
 * 
 *  The CartesianMesh class is responsible for storing and controlling a problem's spatial 
 *  domain information.  First, the mesh stores the global length and area of the domain.
 *  Second, the Mesh also stores the geometric elements, which are (for a one-dimensional
 *  mesh), Zones, Nodes, and Corners.  Together with these geometric elements, the Mesh
 *  stores all of the information necessary to fully describe a problem's spatial domain. */
template<>
class CartesianMesh<OneD> : public CartesianMesh_base<OneD>
{
private:
   /// Defines the base class as the Base type.
   typedef CartesianMesh_base<OneD>    Base;
   
public:
   /// Imports the Zone class from the Base type.
   typedef Base::Zone                  Zone;
   /// Imports the Node class from the Base type.
   typedef Base::Node                  Node;
   /// Imports the Corner class from the Base type.
   typedef Base::Corner                Corner;

private:
   /// Requires the \c Zone type to satisfy the \c ZoneConcept concept.
   BOOST_CLASS_REQUIRE(Zone, , ZoneConcept);
   /// Requires the \c Node type to satisfy the \c NodeConcept concept.
   BOOST_CLASS_REQUIRE(Node, , NodeConcept);
   /// Requires the \c Corner type to satisfy the \c CornerConcept concept.
   BOOST_CLASS_REQUIRE(Corner, , CornerConcept);

public:
   /// Imports the \c LengthType from the \c Base type.
   typedef Base::LengthType            LengthType;
   /// Imports the \c SizeType from the \c Base type.
   typedef Base::SizeType              SizeType;
   /// Imports the \c SweepIterator from the \c Base type.
   typedef Base::SweepIterator         SweepIterator;

   /*! \brief Imports the \c ElementDoesNotExist<Zone> exception class into the 
    *         \c CartesianMesh1D scope. Thrown when an attempt is made to access a 
    *         non-existent \c Zone. */
   typedef ElementDoesNotExist<Zone>    ZoneDoesNotExist;
   /*! \brief Imports the \c ElementDoesNotExist<Node> exception class into the 
    *         \c CartesianMesh1D scope. Thrown when an attempt is made to access a 
    *         non-existent \c Node. */   
   typedef ElementDoesNotExist<Node>    NodeDoesNotExist;
   /*! \brief Imports the \c ElementDoesNotExist<Corner> exception class into the 
    *         \c CartesianMesh1D scope. Thrown when an attempt is made to access a 
    *         non-existent \c Corner. */   
   typedef ElementDoesNotExist<Corner>  CornerDoesNotExist;
   
   /*! \brief Imports the \c BadMeshLength<OneD> exception class into the 
    *         \c CartesianMesh1D scope.  Thrown when an attempt is made to construct
    *         the mesh with a non-positive length. */
   typedef BadMeshLength<OneD>          BadCartMesh1DLength;
   /*! \brief Imports the \c BadMeshAre<OneD> exception class into the 
    *         \c CartesianMesh1D scope.  Thrown when an attempt is made to construct
    *         the mesh with a non-positive area. */   
   typedef BadMeshArea<OneD>            BadCartMesh1DArea;
   /*! \brief Imports the \c ZeroZones exception class into the 
    *         \c CartesianMesh1D scope.  Thrown when an attempt is made to construct
    *         the mesh with zero <tt>Zone</tt>s. */
   typedef ZeroZones                    MeshHasZeroZones;
   /*! \brief Imports the \c NegativeZoneLength<OneD> exception class into the 
    *         \c CartesianMesh1D scope.  Thrown when an attempt is made to construct
    *         the mesh with a non-positive \c Zone length. */   
   typedef NegativeZoneLength<OneD>     NegativeZone1DLength;


	// ******  Mesh Constructors  ******
	/*! \brief Constructor to create a uniform one-dimensional Cartesian Mesh with \a num_zones
    *         <tt>Zone</tt>s. */
	CartesianMesh(LengthType length_in, LengthType area_in, SizeType num_zones,
				     SizeType num_left_ghost_zones = 0.0, bool ghost_left = true,
				     SizeType num_right_ghost_zones = 0.0, bool ghost_right = true,
				     Point<OneD> left_node_location = Point<OneD>(0.0) );
	
	/*! \brief Constructor to create a one-dimensional \c CartesianMesh with variable \c Zone 
	 *         lengths. */
	explicit CartesianMesh(const vector<LengthType>& zone_lengths, LengthType area_in = 1.0,
						        SizeType num_left_ghost_zones = 0, bool ghost_left = true,
				              SizeType num_right_ghost_zones = 0, bool ghost_right = true,
				              Point<OneD> left_node_location = Point<OneD>(0.0) );

   /// Destructor
	~CartesianMesh() {  }

   /*! \brief Returns the area of the mesh.
    *
    *  Returns the area of the mesh.
    *  \return The area of the mesh. */
   LengthType area() const
   {
      return mArea;
   }
   /*! \brief Returns the volume of the mesh.
    *
    *  Returns the volume of the mesh.
    *  \return the volume of the mesh. */
   LengthType volume() const
   {
      return Base::mLength * mArea;
   }   
	
	/*! \brief Returns the number of \c geom_elem objects in the mesh.
	 *
	 *  Returns the number of \c geom_elem geometric element objects in the mesh.
	 *  \par Template Parameters
	 *     <dl> <dt> \e geom_elem </dt>
	 *          <dd> The type of object to return the number of. </dd> </dl>
	 *  \return The number of \c geom_elem objects in the mesh. */
	template<typename geom_elem>
   SizeType numElements() const
   {
      function_requires< GeometricElementConcept<geom_elem> >();
      BOOST_STATIC_ASSERT( geom_elem::DimensionType::value == 1 );
    
      return numCentering<DimensionType, geom_elem>(*this);
   }

	
	// ****** Property Accessors ******	
	// ****** Connectivity Accessors ******
	/*! \brief Given a \c Zone::Id, return the associated \c Zone.
	 *
	 *  Given a \c Zone::Id, returns a constant reference to the associated \c Zone object.
	 *  \param[in] id  Returns the \c Zone corresponding to \a id.
	 *  \return    A constant reference to the \c Zone corresponding to \a id. 
	 *  \throw     ZoneDoesNotExist  Throws \c ZoneDoesNotExist if \a id does not correspond to 
	 *                               a \c Zone in the mesh. */
	const Zone& getZone(const Zone::Id& id) const throw(ZoneDoesNotExist)
	{
#ifndef NMESHCONNECTIVITYCHECK
      if(id.idNumber() >= Base::mZones.size() + mNumLeftGhostZones + mNumRightGhostZones)
         throw ZoneDoesNotExist(__LINE__, __FILE__, id);
#endif
		if( id.idNumber() < Base::mZones.size() ) 
		   return Base::mZones[id.idNumber()];
		else 
		   return (*Base::mGhostZones)[id.idNumber() - Base::mZones.size()];
	}
	/*! \brief Given a \c Node::Id, return the associated \c Node.
	 *
	 *  Given a \c Node::Id, returns a constant reference to the associated \c Node object.
	 *  \param[in] id  Returns the \c Node corresponding to \a id.
	 *  \return    A constant reference to the \c Node corresponding to \a id.
	 *  \throw     NodeDoesNotExist  Throw \c NodeDoesNotExist if \a id does not correspond to 
	 *                               a \c Node in the mesh. */
	const Node& getNode(const Node::Id& id) const throw(NodeDoesNotExist)
	{
#ifndef NMESHCONNECTIVITYCHECK
		// Same number of ghost nodes as ghost zones
      if(id.idNumber() >= Base::mNodes.size() + mNumLeftGhostZones + mNumRightGhostZones)
         throw NodeDoesNotExist(__LINE__, __FILE__, id);
#endif
		if( id.idNumber() < Base::mNodes.size() ) 
		   return Base::mNodes[id.idNumber()];
		else 
		   return (*Base::mGhostNodes)[id.idNumber() - Base::mNodes.size()];
	}
	/*! \brief Given a \c Corner::Id, return the associated \c Corner.
	 *
	 *  Given a \c Corner::Id, returns a constant reference to the associated \c Corner
	 *  object.
	 *  \param[in]  id  Returns the \c Corner corresponding to \a id.
	 *  \return     A constant reference to the \c Corner corresponding to \a id. 
	 *  \throw      CornerDoesNotExist  Throws \c CornerDoesNotExist if \a id does not 
	 *                                  correspond to a \c Corner in the mesh. */
	const Corner& getCorner(const Corner::Id& id) const throw(CornerDoesNotExist)
	{
#ifndef NMESHCONNECTIVITYCHECK
      if( id.idNumber() >= Base::mCorners.size() + 2*(mNumLeftGhostZones+mNumRightGhostZones) )
         throw CornerDoesNotExist(__LINE__, __FILE__, id);
#endif
		if( id.idNumber() < Base::mCorners.size() ) 
		   return Base::mCorners[id.idNumber()];
		else 
		   return (*Base::mGhostCorners)[id.idNumber() - Base::mCorners.size()];
	}
	
	/*! \brief Given a \c Zone::Id, return the associated \c Zone.
	 *
	 *  Given a \c Zone::Id, returns a constant reference to the associated \c Zone object.
	 *  \param[in] id  Returns the \c Zone corresponding to \a id.
	 *  \return    A constant reference to the \c Zone corresponding to \a id.
	 *  \throw     ZoneDoesNotExist  Throws \c ZoneDoesNotExist if \a id does not correspond 
	 *                               to a \c Zone in the mesh. */
	const Zone& operator[](const Zone::Id& id) const throw(ZoneDoesNotExist)
	{
      return getZone(id);
	}
	/*! \brief Given a \c Node::Id, return the associated \c Node.
	 *
	 *  Given a \c Node::Id, returns a constant reference to the associated \c Node object.
	 *  \param[in] id  Returns the \c Node corresponding to \a id.
	 *  \return    A constant reference to the \c Node corresponding to \a id.
	 *  \throw     NodeDoesNotExist  Throws \c NodeDoesNotExist if \a id does not correspond to
	 *                               a \c Node in the mesh. */
	const Node& operator[](const Node::Id& id) const throw(NodeDoesNotExist)
	{
	   return getNode(id);
	}
	/*! \brief Given a \c Corner::Id, return the associated \c Corner.
	 *
	 *  Given a \c Corner::Id, returns a constant reference to the associated \c Corner
	 *  object.
	 *  \param[in] id  Returns the \c Corner corresponding to \a id.
	 *  \return    A constant reference to the \c Corner corresponding to \a id.
	 *  \throw     CornerDoesNotExist  Returns the \c CornerDoesNotExist if \a id does not 
	 *                                 correspond to a \c Corner in the mesh. */
	const Corner& operator[](const Corner::Id& id) const throw(CornerDoesNotExist)
	{
	   return getCorner(id);
	}
	
    //Given a position, return the Zone in which that position is located
    const Zone& getZone(double x) const {
        //Ensure position is inside Mesh
        Assert( x >= 0 );
        Assert( x <= mLength );

        return *(lower_bound( zoneBegin(), zoneEnd(), x, ZoneComp ));

//      const_ZoneIterator zIter = zoneBegin();
//      for( ; zIter != zoneEnd(); ++zIter ){
//          if( zIter->getLeftNode().x() < x and 
//              zIter->getRightNode().x() >= x ) return *zIter;
//      }
        
    }

	// ****** Ghost Element Accessors ***** 
	/*! \brief Returns the number of ghost zones on the left.
	 *
	 *  Returns the number of ghost zones on the left side of the mesh.
	 *  \return  The number of ghost zones on the left side of the mesh. */
	SizeType numLeftGhostZones() const 
	{ 
	   return mNumLeftGhostZones; 
	}
	/*! \brief Returns the number of ghost zones on the right.
	 *
	 *  Returns the number of ghost zones on the right side of the mesh.
	 *  \return  The number of ghost zones on the right side of the mesh. */
	SizeType numRightGhostZones() const 
	{ 
	   return mNumRightGhostZones; 
	}
	/*! \brief Returns the number of ghost nodes on the left.
	 *
	 *  Returns the number of ghost nodes on the left side of the mesh.
	 *  \return  The number of ghost nodes on the left side of the mesh. */
	SizeType numLeftGhostNodes() const 
	{ 
	   return mNumLeftGhostZones; 
	}
	/*! \brief Returns the number of ghost nodes on the right.
	 *
	 *  Returns the number of ghost nodes on the right side of the mesh.
	 *  \return  The number of ghost nodes on the right side of the mesh. */
	SizeType numRightGhostNodes() const 
	{ 
	   return mNumRightGhostZones; 
	} 
	/*! \brief Returns the number of ghost corners on the left.
	 *
	 *  Returns the number of ghost corners on the left side of the mesh.
	 *  \return  The number of ghost corners on the left side of the mesh. */
	SizeType numLeftGhostCorners() const 
	{ 
	   return 2*mNumLeftGhostZones; 
	}
	/*! \brief Returns the number of ghost corners on the right.
	 *
	 *  Returns the number of ghost corners on the right side of the mesh.
	 *  \return  The number of ghost corners on the right side of the mesh. */
	SizeType numRightGhostCorners() const 
	{ 
	   return 2*mNumRightGhostZones; 
	}

	// ****** Sweep Iterator Access ******
	/*! \brief Returns an iterator pointing to the beginning of the sweep pattern.
	 *
	 *  Returns an iterator that points to the \c Zone at the beginning of the sweep
	 *  pattern.
	 *  \param angle  The $S_N$ sweep angle.
	 *  \return       The \c SweepIterator that points to the beginning of the sweep
	 *                pattern for angle \a angle. */
   SweepIterator sweepBegin(const Angle<OneD>& angle) const
	{
		if(angle.mu() > 0) return SweepIterator( angle, mZones, Base::mZones.begin() );
		else return SweepIterator(angle, mZones, Base::mZones.end());
	}
	/*! \brief Returns an iterator pointing to the end of the sweep pattern.
	 *
	 *  Returns an iterator that points to the \c Zone at the end of the sweep
	 *  pattern.
	 *  \param angle  The $S_N$ sweep angle.
	 *  \return       The \c SweepIterator that points to the end of the sweep
	 *                pattern for angle \a angle. */
   SweepIterator sweepEnd(const Angle<OneD>& angle) const
	{
		if(angle.mu() > 0) return SweepIterator(angle, mZones, Base::mZones.end());
		else return SweepIterator(angle, mZones, Base::mZones.begin());
	}

private:
   /// The default constructor is private to prevent its use.
   CartesianMesh<OneD>();
   /// The copy constructor is private to prevent its use.
   CartesianMesh<OneD>(CartesianMesh<OneD>&);
   /// The copy operator is private to prevent its use.
   void operator=(CartesianMesh<OneD>&);
   
   /// Check the constructor input data with some common sense checking.
   void mDoCommonSenseChecks(SizeType num_zones) 
                 throw(BadCartMesh1DLength, BadCartMesh1DArea, 
                       MeshHasZeroZones);
   /// Check the constructor input data with some common sense checking.
	void mDoCommonSenseChecks(const vector<LengthType>& zone_lengths)
	              throw(BadCartMesh1DLength, BadCartMesh1DArea, 
	                    MeshHasZeroZones, NegativeZone1DLength);
	   
	// ****** Build the mesh ******
	/// Creates the real geometric elements.
	void mCreateGeometricElements(const vector<LengthType>& zone_lengths,
	                              const Point<OneD>& left_node_location);
	/// Sets the connectivity of the real geometric elements.
	void mConnectGeometricElements();
	/// Creates the ghost elements.
	void mCreateGhostElements(bool ghost_left, bool ghost_right);
	/// Sets the connectivity of the left ghost elements.
	void mConnectLeftGhostElements();
	/// Sets the connectivity of the right ghost elements.
	void mConnectRightGhostElements();
	
	// ****** Private Mesh Data ******
   /// Cross-sectional area of the Mesh.
	LengthType mArea;
	
	// Ghost Zone Information
   /// The number of ghost zones on the left.
	SizeType mNumLeftGhostZones;
   /// The number of ghost zones on the right.
	SizeType mNumRightGhostZones;               
};

/// Define the one-dimensional \c CartesianMesh as \c CartMesh1D.
typedef CartesianMesh<OneD>  CartMesh1D;
/// Define the one-dimensional \c Zone type as \c Zone1D.
typedef Zone<OneD>           Zone1D;
/// Define the one-dimensional \c Node type as \c Node1D.
typedef Node<OneD>           Node1D;
/// Define the one-dimensional \c Corner type as \c Corner1D.
typedef Corner<OneD>         Corner1D;

/*! \brief The \c Zone template specialization for the \c numCentering 
 *         function.
 *
 *  This \c numCentering function specialization is specialized on
 *  \c Zone.  It returns the number of <tt>Zone</tt>s in the mesh.
 *  \param[in]  mesh  A reference to the one-dimensional \c CartesianMesh.
 *  \return     The number of <tt>Zone</tt>s in the \c CartesianMesh. */
template<>
inline CartMesh1D::SizeType 
numCentering< OneD, Zone<OneD> >(const CartMesh1D& mesh)
{
    return mesh.numZones();
}

/*! \brief The \c Node template specialization for the \c numCentering 
 *         function.
 *
 *  This \c numCentering function specialization is specialized on
 *  \c Node.  It returns the number of <tt>Node</tt>s in the mesh.
 *  \param[in] mesh  A reference to the one-dimensional \c CartesianMesh.
 *  \return    The number of <tt>Node</tt>s in the \c CartesianMesh. */
template<>
inline CartMesh1D::SizeType 
numCentering< OneD, Node<OneD> >(const CartMesh1D& mesh)
{
    return mesh.numNodes();
}

/*! \brief The \c Corner template specialization for the \c numCentering 
 *         function.
 *
 *  This \c numCentering function specialization is specialized on
 *  \c Corner.  It returns the number of <tt>Corner</tt>s in the mesh.
 *  \param[in] mesh  A reference to the one-dimensional \c CartesianMesh.
 *  \return    The number of <tt>Corner</tt>s in the \c CartesianMesh. */
template<>
inline CartMesh1D::SizeType 
numCentering< OneD, Corner<OneD> >(const CartMesh1D& mesh)
{
	return mesh.numCorners();
}


/// @}

inline bool ZoneComp( const Zone<OneD>& Z, double x){
    if (Z.getRightNode().x() < x) return true;
    else return false;
}

#endif
