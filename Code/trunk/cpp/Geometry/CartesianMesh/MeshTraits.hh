// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file MeshTraits.hh
 *  \author Greg Davidson
 *  \date June 3, 2005
 *  
 *  \brief Defines the types universal to <tt>CartesianMesh</tt>'s of all spatial 
 *         dimensions.
 *
 *  This file defines the types universal to <tt>CartesianMesh</tt>'s of all spatial 
 *  dimensions. */
 
#ifndef MESHTRAITS_HH
#define MESHTRAITS_HH

// ****** Forward Declaration of Mesh-Dependent Types ******
/// Forward declaration of the Zone template class.
template<typename dimension_type> class Zone;
/// Forward declaration of the Node template class.
template<typename dimension_type> class Node;
/// Forward declaration of the Corner template class.
template<typename dimension_type> class Corner;
/// Forward declaration of the Point template class.
template<typename dimension_type> class Point;
/// Forward declaration of the SweepIterator template class.
template<typename dimension_type, typename container_type> class SweepIterator;
/// Forward declaration of the GhostElement template class.
template<typename geom_elem> class GhostElement;

#include <boost/ptr_container/ptr_vector.hpp>

#include "Dimension.hh"
#include "Iterator.hh"
#include "ReverseIterator.hh"

using boost::ptr_vector;

/*! \addtogroup MeshMod Mesh Subsystem
 *  @{  */

/*! \brief The \c MeshTraits class provides all of the type definitions needed by general 
 *         <tt>CartesianMesh</tt>es.
 *
 *  The \c MeshTraits template class provides all of the type definitions needed by the
 *  \c CartesianMesh.  By extracting these and placing them in a different class, we simplify 
 *  the \c CartesianMesh interface.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> A type giving the spatial dimension spanned by the \c CartesianMesh.  
 *               This type must satisfy the \c DimensionConcept concept requirement. </dd> </dl> */
template<typename dimension_type>
class MeshTraits
{
private:
   /// Require that \c dimension_type fulfill the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);

public:
   // ****** Define the basic types for meshes ******
   /// Alias the \c dimension_type as \c DimensionType.
   typedef dimension_type        DimensionType;
   /// Define the \c LengthType for storing \c CartesianMesh lengths.   
   typedef Real8                 LengthType;
   /// Define the \c SizeType for storing sizes associated with the \c CartesianMesh.
   typedef UnsignedInt4          SizeType;
   /// Define an \c IndexType to use for indexing.
   typedef UnsignedInt4          IndexType;
   /// Define the \c PointType for storing spatial locations.
   typedef Point<DimensionType>  PointType;

   // ****** Define the basic geometric element types ******
   /// Defines the \c Zone geometric element type.
   typedef Zone<DimensionType>   Zone;
   /// Defines the \c Node geometric element type.
   typedef Node<DimensionType>   Node;
   /// Defines the \c Corner geometric element type.
   typedef Corner<DimensionType> Corner;
   /// Defines the \c GhostZone geometric element type.
   typedef GhostElement<Zone>    GhostZone;
   /// Defines the \c GhostNode geometric element type.
   typedef GhostElement<Node>    GhostNode;
   /// Defines the \c GhostCorner geometric element type.
   typedef GhostElement<Corner>  GhostCorner;


   // ****** Define the CartesianMesh Iterator types ******
   /// Defines an \c Iterator over the <tt>CartesianMesh</tt>'s <tt>Zone</tt>s.
   typedef Iterator< ptr_vector<Zone>, 
                     typename ptr_vector<Zone>::iterator >   ZoneIterator;
   /// Defines a const_<tt>Iterator</tt> over the <tt>CartesianMesh</tt>'s <tt>Zone</tt>s.
   typedef Iterator< ptr_vector<Zone> >                      const_ZoneIterator;
   /// Defines an \c Iterator over the <tt>CartesianMesh</tt>'s <tt>Node</tt>s.
   typedef Iterator< ptr_vector<Node>, 
                     typename ptr_vector<Node>::iterator >   NodeIterator;
   /// Defines a const_<tt>Iterator</tt> over the <tt>CartesianMesh</tt>'s <tt>Node</tt>s.
   typedef Iterator< ptr_vector<Node> >                      const_NodeIterator;
   /// Defines an \c Iterator over the <tt>CartesianMesh</tt>'s <tt>Corner</tt>s.
   typedef Iterator< ptr_vector<Corner>, 
	                  typename ptr_vector<Corner>::iterator > CornerIterator;
   /// Defines a const_<tt>Iterator</tt> over the <tt>CartesianMesh</tt>'s <tt>Corner</tt>s.
   typedef Iterator< ptr_vector<Corner> >                    const_CornerIterator;

   // ****** Define the CartesianMesh ReverseIterator types ******
   /// Defines a \c ReverseIterator over the <tt>CartesianMesh</tt>'s <tt>Zone</tt>s.
   typedef ReverseIterator< ZoneIterator >                   reverse_ZoneIterator;
   /// Defines a const_<tt>ReverseIterator</tt> over the <tt>CartesianMesh</tt>'s <tt>Zone</tt>s.
   typedef ReverseIterator< const_ZoneIterator >             const_reverse_ZoneIterator;
   /// Defines a \c ReverseIterator over the <tt>CartesianMesh</tt>'s <tt>Node</tt>s.
   typedef ReverseIterator< NodeIterator >                   reverse_NodeIterator;
   /// Defines a const_<tt>ReverseIterator</tt> over the <tt>CartesianMesh</tt>'s <tt>Node</tt>s.
   typedef ReverseIterator< const_NodeIterator >             const_reverse_NodeIterator;
   /// Defines a \c ReverseIterator over the <tt>CartesianMesh</tt>'s <tt>Corner</tt>s.
   typedef ReverseIterator< CornerIterator >                 reverse_CornerIterator;
   /// Defines a const_<tt>ReverseIterator</tt> over the <tt>CartesianMesh</tt>'s <tt>Corner</tt>s.
   typedef ReverseIterator< const_CornerIterator >           const_reverse_CornerIterator;
    
   // ****** Define the CartesianMesh SweepIterator ******
   /// Defines a \c SweepIterator for sweeping over the <tt>CartesianMesh</tt>'s <tt>Zone</tt>s.
   typedef SweepIterator<DimensionType, ptr_vector<Zone> >   SweepIterator;
};

/// @}

#endif

