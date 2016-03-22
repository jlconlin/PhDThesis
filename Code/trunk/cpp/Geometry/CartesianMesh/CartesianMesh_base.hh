// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file CartesianMesh_base.hh
 *  \author Greg Davidson
 *	 \date June 3, 2005
 *
 *  \brief Provides the functionality that is common to \c CartesianMesh classes of all
 *         dimensions.
 *
 *  This file provides the \c CartesianMesh_base class.  This class contains all of the 
 *  functionality that is common to \c CartesianMesh classes of all spatial dimensions and 
 *  is not dependent on complete interfaces of the geometric types. */

#ifndef CARTESIANMESH_BASE_HH
#define CARTESIANMESH_BASE_HH

#include <boost/ptr_container/ptr_vector.hpp>

#include "Types.hh"
#include "Dimension.hh"
#include "GeomId.hh"
#include "MeshTraits.hh"
#include "Assert.hh"
#include "GeometricElementConcept.hh"

/// \defgroup MeshMod Mesh Module
/*! \addtogroup MeshMod Mesh Module
 *  \brief The Mesh Module contains all of the geometric functionality.
 *
 *  The Mesh Module consists of all of the classes and functionality necessary to
 *  fully describe and control the spatial problem domain.
 *  @{  */

using boost::ptr_vector;

/*! \brief Returns the number of \c geom_elem objects in the \c CartesianMesh.
 *
 *  Returns the number of \c geom_elem objects of spatial dimension
 *  \c dimension_type in the \c CartesianMesh.  The general
 *  function is not implemented, only the template specializations
 *  are implemented.
 *  \par Template Parameters:
 *  <dl> <dt> \e dimension_type </dt>
 *       <dd> Properly, the dimension_type should be one of the \c Dimension template 
 *            classes.  This is enforced using the \c DimensionConcept concept check. 
 *            This represents the number of spatial dimensions the mesh spans. </dd> 
 *       <dt> \e geom_elem </dt>
 *       <dd> This refers to a geometric element, such as \c Zone, \c Node or
 *            \c Corner. </dd> </dl> */
template<typename dimension_type, typename geom_elem>
inline typename CartesianMesh<dimension_type>::SizeType 
numCentering(const CartesianMesh<dimension_type>& mesh);



/*! \brief Provides the functionality that is common to \c CartesianMesh classes of all
 *         dimensions.
 *
 *  This class contains all of the functionality that is common to \c CartesianMesh 
 *  classes of all spatial dimensions and is not dependent on complete interfaces 
 *  of the geometric types.
 *  \par Template Parameters:
 *    <dl> <dt> \e dimension_type </dt>  
 *         <dd> Properly, the dimension_type should be one of the \c Dimension template 
 *              classes.  This is enforced using the \c DimensionConcept concept check. 
 *              This represents the number of spatial dimensions the mesh spans. </dd> </dl> */
template<typename dimension_type>
class CartesianMesh_base : public MeshTraits<dimension_type>, 
                           boost::noncopyable
{
private:
   /*! \brief Checks at compile time that the dimension_type satisfies the
    *         \c DimensionConcept. */
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);

public:
   /// Defines the dimension_type template parameter as the \c DimensionType.
   typedef dimension_type                                DimensionType;
   /// Aliases the base class.
   typedef MeshTraits<DimensionType>                     Base;
   
public:
   // ****** Return basic mesh properties ******
	/*! \brief Returns the length (x-axis) of the \c CartesianMesh.
	 *
	 *  Returns the length (x-axis) of the \c CartesianMesh.
	 *  \return The length of the c CartesianMesh. */
	typename Base::LengthType length() const 
	{ 
	   return mLength; 
	}
   /*! \brief Returns the total number of <tt>Zone</tt>s in the \c CartesianMesh.
    *
    *  Returns the total number of <tt>Zone</tt>s, including both real and 
    *  ghost <tt>Zone</tt>s in the \c CartesianMesh.
    *  \return The number of <tt>Zone</tt>s in the \c CartesianMesh. */
   typename Base::SizeType numZones() const
   {
      return mZones.size() + numGhostZones();
   }
   /*! \brief Returns the total number of <tt>Node</tt>s in the \c CartesianMesh.
    *
    *  Returns the total number of <tt>Node</tt>s, including both real and 
    *  ghost <tt>Node</tt>s in the \c CartesianMesh.
    *  \return The number of <tt>Node</tt>s in the \c CartesianMesh. */
   typename Base::SizeType numNodes() const
   {
      return mNodes.size() + numGhostNodes();
   }
   /*! \brief Returns the total number of <tt>Corner</tt>s in the \c CartesianMesh.
    *
    *  Returns the total number of <tt>Corner</tt>s, including both real and 
    *  ghost <tt>Corner</tt>s in the \c CartesianMesh.
    *  \return The number of <tt>Corner</tt>s in the \c CartesianMesh. */
   typename Base::SizeType numCorners() const
   {
      return mCorners.size() + numGhostCorners();
   }
   /*! \brief Returns the number of <tt>GhostZone</tt>s in the \c CartesianMesh.
    *
    *  Returns the number of <tt>GhostZone</tt>s in the \c CartesianMesh.
    *  \return The number of <tt>GhostZone</tt>s in the \c CartesianMesh. */
   typename Base::SizeType numGhostZones() const
	{
	   if(mGhostZones == 0) 
	      return 0;
	   else
	      return mGhostZones->size();
   }
   /*! \brief Returns the number of <tt>GhostNode</tt>s in the \c CartesianMesh.
    *
    *  Returns the number of <tt>GhostNode</tt>s in the \c CartesianMesh.
    *  \return The number of <tt>GhostNode</tt>s in the \c CartesianMesh. */
   typename Base::SizeType numGhostNodes() const
   {
      if(mGhostNodes == 0)
         return 0;
      else
         return mGhostNodes->size();
   }
   /*! \brief Returns the number of <tt>GhostCorner</tt>s in the \c CartesianMesh.
    *
    *  Returns the number of <tt>GhostCorner</tt>s in the \c CartesianMesh.
    * \return The number of <tt>GhostCorner</tt>s in the \c CartesianMesh. */
   typename Base::SizeType numGhostCorners() const
   {
      if(mGhostCorners == 0)
         return 0;
      else
         return mGhostCorners->size();
   }   
	
	// ****** Zone Forward Iterator Access ******
	/*! \brief Returns a \c ZoneIterator pointing to first real \c Zone.
	 *
	 *  Returns a \c ZoneIterator pointing to the first real \c Zone (\c Zone 0).
	 *  \return A \c ZoneIterator pointing to the first real \c Zone. */
	typename Base::ZoneIterator zoneBegin() 
	{ 
	   return typename Base::ZoneIterator( mZones, mZones.begin() ); 
	}
	/*! \brief Returns a \c ZoneIterator pointing past the last real \c Zone.
	 *
	 *  Returns a \c ZoneIterator pointing to one position past the last
	 *  real \c Zone.
	 *  \return A \c ZoneIterator pointing to one position past the last
	 *          real \c Zone. */
	typename Base::ZoneIterator zoneEnd() 
	{ 
	   return typename Base::ZoneIterator( mZones, mZones.end() ); 
	}
	/*! \brief Returns a \c const_ZoneIterator pointing to first real \c Zone.
	 *
	 *  Returns a \c const_ZoneIterator pointing to the first real \c Zone 
	 *  (\c Zone 0).
	 *  \return A \c const_ZoneIterator pointing to the first real \c Zone. */
	typename Base::const_ZoneIterator zoneBegin() const 
	{ 
	   return typename Base::const_ZoneIterator( mZones, mZones.begin() ); 
	}
	/*! \brief Returns a \c const_ZoneIterator pointing past the last real \c Zone.
	 *
	 *  Returns a \c const_ZoneIterator pointing to one position past the last
	 *  real \c Zone.
	 *  \return A \c const_ZoneIterator pointing to one position past the last
	 *          real \c Zone. */
	typename Base::const_ZoneIterator zoneEnd() const 
	{ 
	   return typename Base::const_ZoneIterator(mZones, mZones.end()); 
	}
	
	// ****** Node Forward Iterator Access ******
	/*! \brief Returns a \c NodeIterator pointing to first real \c Node.
	 *
	 *  Returns a \c NodeIterator pointing to the first real \c Node 
	 *  (\c Node 0).
	 *  \return A \c NodeIterator pointing to the first real \c Node. */
	typename Base::NodeIterator nodeBegin() 
	{ 
	   return typename Base::NodeIterator( mNodes, mNodes.begin() ); 
	}
	/*! \brief Returns a \c NodeIterator pointing past the last real \c Node.
	 *
	 *  Returns a \c NodeIterator pointing to one position past the last
	 *  real \c Node.
	 *  \return A \c NodeIterator pointing to one position past the last
	 *          real \c Node. */
	typename Base::NodeIterator nodeEnd() 
	{ 
	   return typename Base::NodeIterator( mNodes, mNodes.end() ); 
	}
	/*! \brief Returns a \c NodeIterator pointing to first real \c Node.
	 *
	 *  Returns a \c NodeIterator pointing to the first real \c Node 
	 *  (\c Node 0).
	 *  \return A \c NodeIterator pointing to the first real \c Node. */
	typename Base::const_NodeIterator nodeBegin() const 
	{ 
	   return typename Base::const_NodeIterator( mNodes, mNodes.begin() ); 
	}
	/*! \brief Returns a \c const_NodeIterator pointing past the last real \c Node.
	 *
	 *  Returns a \c const_NodeIterator pointing to one position past the last
	 *  real \c Node.
	 *  \return A \c const_NodeIterator pointing to one position past the last
	 *          real \c Node. */
	typename Base::const_NodeIterator nodeEnd() const 
	{ 
	   return typename Base::const_NodeIterator( mNodes, mNodes.end() ); 
	}	
	
	// ****** Corner Forward Iterator Access ******
	/*! \brief Returns a \c CornerIterator pointing to first real \c Corner.
	 *
	 *  Returns a \c CornerIterator pointing to the first real \c Corner 
	 *  (\c Corner 0).
	 *  \return A \c CornerIterator pointing to the first real \c Corner. */
	typename Base::CornerIterator cornerBegin() 
	{ 
	   return typename Base::CornerIterator( mCorners, mCorners.begin() ); 
	}
	/*! \brief Returns a \c CornerIterator pointing past the last real \c Corner.
	 *
	 *  Returns a c CornerIterator pointing to one position past the last
	 *  real c Corner.
	 *  \return A \c CornerIterator pointing to one position past the last
	 *          real \c Corner. */
	typename Base::CornerIterator cornerEnd() 
	{ 
	   return typename Base::CornerIterator( mCorners, mCorners.end() ); 
	}
	/*! \brief Returns a \c const_CornerIterator pointing to first real \c Corner.
	 *
	 *  Returns a \c const_CornerIterator pointing to the first real \c Corner 
	 *  (\c Corner 0).
	 *  \return A \c const_CornerIterator pointing to the first real \c Corner. */
	typename Base::const_CornerIterator cornerBegin() const 
	{ 
	   return typename Base::const_CornerIterator( mCorners, mCorners.begin() ); 
	}
	/*! \brief Returns a \c const_CornerIterator pointing past the last real \c Corner.
	 *
	 *  Returns a \c const_CornerIterator pointing to one position past the last
	 *  real \c Corner.
	 *  \return A \c const_CornerIterator pointing to one position past the last
	 *          real \c Corner. */
	typename Base::const_CornerIterator cornerEnd() const 
	{ 
	   return typename Base::const_CornerIterator( mCorners, mCorners.end() ); 
	}	
	
	// ****** Zone Reverse Iterator Access ******
	/*! \brief Returns a \c reverse_ZoneIterator pointing to the last real \c Zone.
	 *
	 *  Returns a \c reverse_ZoneIterator pointing to the last real \c Zone 
	 *  (\c Zone 0).
	 *  \return A \c reverse_ZoneIterator pointing to the last real \c Zone. */
	typename Base::reverse_ZoneIterator zoneRBegin() 
	{ 
	   return typename Base::reverse_ZoneIterator( zoneEnd() ); 
	}
	/*! \brief Returns a \c reverse_ZoneIterator pointing past the first real \c Zone.
	 *
	 *  Returns a \c reverse_ZoneIterator pointing to one position past the first 
	 *  real \c Zone (\c Zone 0).
	 *  \return A \c reverse_ZoneIterator pointing to one position past the first 
	 *          real \c Zone. */
	typename Base::reverse_ZoneIterator zoneREnd() 
	{ 
	   return typename Base::reverse_ZoneIterator( zoneBegin() ); 
	}
	/*! \brief Returns a \c const_reverse_ZoneIterator pointing to the last real \c Zone.
	 *
	 *  Returns a \c reverse_ZoneIterator pointing to the last real \c Zone (\c Zone 0).
	 *  \return A \c reverse_ZoneIterator pointing to the last real Zone. */
	typename Base::const_reverse_ZoneIterator zoneRBegin() const 
	{ 
	   return typename Base::const_reverse_ZoneIterator( zoneEnd() ); 
	}
	/*! \brief Returns a \c const_reverse_ZoneIterator pointing past the first real \c Zone.
	 *
	 *  Returns a \c const_reverse_ZoneIterator pointing to one position past the first 
	 *  real \c Zone (\c Zone 0).
	 *  \return A \c const_reverse_ZoneIterator pointing to one position past the first 
	 *          real \c Zone. */
	typename Base::const_reverse_ZoneIterator zoneREnd() const 
	{ 
	   return typename Base::const_reverse_ZoneIterator( zoneBegin() ); 
	}
	
	// ****** Node Reverse Iterator Access ******
	/*! \brief Returns a \c reverse_NodeIterator pointing to the last real \c Node.
	 *
	 *  Returns a \c reverse_NodeIterator pointing to the last real \c Node (\c Node 0).
	 *  \return A \c reverse_NodeIterator pointing to the last real \c Node. */
	typename Base::reverse_NodeIterator nodeRBegin() 
	{ 
	   return typename Base::reverse_NodeIterator( nodeEnd() ); 
	}
	/*! \brief Returns a \c reverse_NodeIterator pointing past the first real \c Node.
	 *
	 *  Returns a \c reverse_NodeIterator pointing to one position past the first 
	 *  real \c Node (\c Node 0).
	 *  \return A \c reverse_NodeIterator pointing to one position past the first 
	 *          real \c Node. */
	typename Base::reverse_NodeIterator nodeREnd() 
	{ 
	   return typename Base::reverse_NodeIterator( nodeBegin() ); 
	}
	/*! \brief Returns a \c const_reverse_NodeIterator pointing to the last real \c Node.
	 *
	 *  Returns a \c const_reverse_NodeIterator pointing to the last real \c Node 
	 *  (\c Node 0).
	 *  \return A \c const_reverse_NodeIterator pointing to the last real \c Node. */
	typename Base::const_reverse_NodeIterator nodeRBegin() const 
	{ 
	   return typename Base::const_reverse_NodeIterator( nodeEnd() ); 
	}
	/*! \brief Returns a \c const_reverse_NodeIterator pointing past the first real 
	 *         \c Node.
	 *
	 *  Returns a \c const_reverse_NodeIterator pointing to one position past the first 
	 *  real \c Node (\c Node 0).
	 *  \return A \c const_reverse_NodeIterator pointing to one position past the first 
	 *          real \c Node. */
	typename Base::const_reverse_NodeIterator nodeREnd() const 
	{ 
	   return typename Base::const_reverse_NodeIterator( nodeBegin() ); 
	}
	
	// ****** Corner Reverse Iterator Access ******
	/*! \brief Returns a \c reverse_CornerIterator pointing to the last real \c Corner.
	 *
	 *  Returns a \c reverse_CornerIterator pointing to the last real \c Corner 
	 *  (\c Corner 0).
	 *  \return A \c reverse_CornerIterator pointing to the last real \c Corner. */
	typename Base::reverse_CornerIterator cornerRBegin() 
	{ 
	   return typename Base::reverse_CornerIterator( cornerEnd() ); 
	}
	/*! \brief Returns a \c reverse_CornerIterator pointing past the first real \c Corner.
	 *
	 *  Returns a \c reverse_CornerIterator pointing to one position past the first 
	 *  real \c Corner (\c Corner 0).
	 *  \return A \c reverse_CornerIterator pointing to one position past the first 
	 *          real \c Corner. */
	typename Base::reverse_CornerIterator cornerREnd() 
	{ 
	   return typename Base::reverse_CornerIterator( cornerBegin() ); 
	}
	/*! \brief Returns a \c const_reverse_CornerIterator pointing to the last real \c Corner.
	 *
	 *  Returns a \c const_reverse_CornerIterator pointing to the last real \c Corner 
	 *  (\c Corner 0).
	 *  \return A \c const_reverse_CornerIterator pointing to the last real \c Corner. */
	typename Base::const_reverse_CornerIterator cornerRBegin() const 
	{ 
	   return typename Base::const_reverse_CornerIterator( cornerEnd() ); 
	}
	/*! \brief Returns a \c const_reverse_CornerIterator pointing past the first real 
	 *         \c Corner.
	 *
	 *  Returns a \c const_reverse_CornerIterator pointing to one position past the first 
	 *  real \c Corner (\c Corner 0).
	 *  \return A \c const_reverse_CornerIterator pointing to one position past the first 
	 *          real \c Corner. */
	typename Base::const_reverse_CornerIterator cornerREnd() const 
	{ 
	   return typename Base::const_reverse_CornerIterator( cornerBegin() ); 
	}

protected:
   /*! \brief Constructor.  Protected to prevent user instantiation of \c CartesianMesh_base.
    *
    *  Constructor.  This constructor is protected so that it can only be instantiated by
    *  its daughter classes and not by the user.
    *  \param[in] length_in          The length of the \c CartesianMesh.
    *  \param[in] num_zones          The number of real zones in the \c CartesianMesh.
    *  \param[in] num_nodes          The number of real nodes in the \c CartesianMesh.
    *  \param[in] num_corners        The number of real corners in the \c CartesianMesh.
    *  \param[in] num_ghost_zones    The number of ghost zones in the \c CartesianMesh.
    *  \param[in] num_ghost_nodes    The number of ghost nodes in the \c CartesianMesh.
    *  \param[in] num_ghost_corners  The number of ghost corners in the \c CartesianMesh. */
   CartesianMesh_base(typename Base::LengthType length_in, 
                      typename Base::SizeType num_zones,
                      typename Base::SizeType num_nodes, 
                      typename Base::SizeType num_corners,
                      typename Base::SizeType num_ghost_zones, 
                      typename Base::SizeType num_ghost_nodes,
                      typename Base::SizeType num_ghost_corners)
      : mLength(length_in), mZones(num_zones), 
        mNodes(num_nodes), mCorners(num_corners),
        mGhostZones(0), mGhostNodes(0), mGhostCorners(0)
   {
      if(num_ghost_zones != 0)
      {
         mGhostZones = new ptr_vector<typename Base::GhostZone>(num_ghost_zones);
      }
      if(num_ghost_nodes != 0)
      {
         mGhostNodes = new ptr_vector<typename Base::GhostNode>(num_ghost_nodes);
      }
      if(num_ghost_corners != 0)
      {
         mGhostCorners = new ptr_vector<typename Base::GhostCorner>(num_ghost_corners);
      }
   }  

   /// \brief Destructor.  Destroys the ghost elements (if they exist).
   ~CartesianMesh_base()
   {
      if(mGhostZones != 0)
      {
         delete mGhostZones;
      }
      if(mGhostNodes != 0)
      {
         delete mGhostNodes;
      }
      if(mGhostCorners != 0)
      {
         delete mGhostCorners;
      }
   }

   /// The length of the \c CartesianMesh.
	typename Base::LengthType mLength;

   /// A \c ptr_vector storing the <tt>Zone</tt>s.
	ptr_vector<typename Base::Zone> mZones;
	/// A \c ptr_vector storing the <tt>Node</tt>s.
	ptr_vector<typename Base::Node> mNodes;
	/// A \c ptr_vector storing the <tt>Corner</tt>s.
	ptr_vector<typename Base::Corner> mCorners;
	
	/// A pointer to a (possible) heap allocated \c ptr_vector for <tt>GhostZone</tt>s.
	ptr_vector<typename Base::GhostZone>* mGhostZones;
	/// A pointer to a (possible) heap allocated \c ptr_vector for <tt>GhostNode</tt>s.
	ptr_vector<typename Base::GhostNode>* mGhostNodes;
	/// A pointer to a (possible) heap allocated \c ptr_vector for <tt>GhostCorner</tt>s.
	ptr_vector<typename Base::GhostCorner>* mGhostCorners;

private:
   /// The default constructor is private to prevent default instantiation.
   CartesianMesh_base();
};




/*! \brief The \c CartesianMesh class is responsible for storing and controlling a 
 *         problem's spatial domain information.
 * 
 *  The CartesianMesh class is responsible for storing and controlling a problem's spatial 
 *  domain information.  This class prevents the instantiation of arbitrary-dimensional
 *  <tt>CartesianMesh</tt>es by making the constructor private. 
 *  \par Template Parameters:
 *    <dl> <dt> \e dimension_type </dt>  
 *         <dd> Properly, the dimension_type should be one of the \c Dimension template 
 *              classes.  This is enforced using the \c DimensionConcept concept check. 
 *              This represents the number of spatial dimensions the mesh spans. </dd> </dl> */
template<typename dimension_type>
class CartesianMesh : CartesianMesh_base<dimension_type>, 
                      boost::noncopyable
{  
private:
   /// The default constructor is private to prevent instantiation.
   CartesianMesh();
};



#endif

