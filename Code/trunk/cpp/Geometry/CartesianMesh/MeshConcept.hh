// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file MeshConcept.hh
 *  \author Greg Davidson
 *  \date November 6, 2006
 *  
 *  \brief Provides the \c CartesianMeshConcept concept class.
 * 
 *  This file provides the \c CartesianMeshConcept concept class.  This is provided to 
 *  require that all \c CartesianMesh types provide certain minimal functionality. */

#ifndef MESHCONCEPT_HH
#define MESHCONCEPT_HH

#include <boost/concept_check.hpp>

#include "Types.hh"
#include "Dimension.hh"

using boost::function_requires;
using boost::ConvertibleConcept;
using boost::UnsignedIntegerConcept;

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/*! \brief Depicts the functionality that all \c CartesianMesh types must implement.
 *
 *  The \c CartesianMeshConcept class template dictates what functionality
 *  all \c CartesianMesh types must implement.
 *  \par Template Parameters: 
 *     <dl> <dt> \e mesh_type </dt> 
 *          <dd> The mesh type to check for concept conformity. </dd> </dl> 
 *  \par Concept Requirements:
 *       The following types must be provided by the \c mesh_type type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c DimensionType </TD> 
 *                <TD> The \c mesh_type type must provide a \c DimensionType. </TD> </TR>
 *          <TR>  <TD> \arg \c LengthType </TD> 
 *                <TD> All \c mesh_type types must provide a \c LengthType. </TD> </TR>
 *          <TR>  <TD> \arg \c SizeType </TD> 
 *                <TD> All \c mesh_type types must provide a \c SizeType. </TD> </TR>
 *          <TR>  <TD> \arg \c Zone </TD> 
 *                <TD> All \c mesh_type types must provide a \c Zone. </TD> </TR>
 *          <TR>  <TD> \arg \c Node </TD> 
 *                <TD> All \c mesh_type types must provide a \c Node. </TD> </TR>
 *          <TR>  <TD> \arg \c Corner </TD> 
 *                <TD> All \c mesh_type types must provide a \c Corner. </TD> </TR>
 *          <TR>  <TD> \arg \c const_ZoneIterator </TD> 
 *                <TD> All \c mesh_type types must provide a \c const_ZoneIterator 
 *                                                                            type. </TD> </TR>
 *          <TR>  <TD> \arg \c const_NodeIterator </TD> 
 *                <TD> All \c mesh_type types must provide a \c const_NodeIterator 
 *                                                                            type. </TD> </TR>
 *          <TR>  <TD> \arg \c const_CornerIterator </TD> 
 *                <TD> All \c mesh_type types must provide a \c const_CornerIterator 
 *                                                                             type. </TD> </TR>
 *          <TR>  <TD> \arg \c SweepIterator </TD> 
 *                <TD> All \c mesh_type types must provide a \c SweepIterator 
 *                                                                             type. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c mesh_type type:
 *       \arg The \c DimensionType must satisfy the \c DimensionConcept concept.
 *       \arg The \c LengthType type must be convertible to a \c Real8 type.
 *       \arg The \c SizeType type must satisfy the UnsignedIntegerConcept concept.
 *       \arg The \c mesh_type must provide a \c length() accessor.
 *       \arg The \c mesh_type must provide a \c area() accessor. 
 *       \arg The \c mesh_type must provide a \c volume() accessor.
 *       \arg The \c mesh_type must provide a \c numZones() accessor.
 *       \arg The \c mesh_type must provide a \c numNodes() accessor. 
 *       \arg The \c mesh_type must provide a \c numCorners() accessor. 
 *       \arg The \c mesh_type must provide a \c getZone(Zone::Id) method.
 *       \arg The \c mesh_type must provide a \c getNode(Node::Id) method.
 *       \arg The \c mesh_type must provide a \c getCorner(Corner::Id) method.
 *       \arg The \c mesh_type must provide a \c zoneBegin() method.
 *       \arg The \c mesh_type must provide a \c zoneEnd() method.
 *       \arg The \c mesh_type must provide a \c nodeBegin() method.
 *       \arg The \c mesh_type must provide a \c nodeEnd() method.
 *       \arg The \c mesh_type must provide a \c cornerBegin() method.
 *       \arg The \c mesh_type must provide a \c cornerEnd() method.
 *       \arg The \c mesh_type must provide a \c sweepBegin(<tt>Angle</tt><\c DimensionType>) method.
 *       \arg The \c mesh_type must provide a \c sweepEnd(<tt>Angle</tt>< \c DimensionType>) method.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename mesh_type>
class CartesianMeshConcept
{
public:
   // ****** Defines the required mesh types ******
   /// The \c mesh_type must provide a \c DimensionType type.
   typedef typename mesh_type::DimensionType          DimensionType;
   /// The \c mesh_type must provide a \c LengthType type.
   typedef typename mesh_type::LengthType             LengthType;
   /// The \c mesh_type must provide a \c SizeType type.
   typedef typename mesh_type::SizeType               SizeType;
   /// The \c mesh_type must provide a \c Zone type.
   typedef typename mesh_type::Zone                   Zone;
   /// The \c mesh_type must provide a \c Node type.
   typedef typename mesh_type::Node                   Node;
   /// The \c mesh_type must provide a \c Corner type.
   typedef typename mesh_type::Corner                 Corner;
   /// The \c mesh_type must provide a \c const_ZoneIterator type.
   typedef typename mesh_type::const_ZoneIterator     const_ZoneIterator;
   /// The \c mesh_type must provide a \c const_NodeIterator type.
   typedef typename mesh_type::const_NodeIterator     const_NodeIterator;
   /// The \c mesh_type must provide a \c const_CornerIterator type.
   typedef typename mesh_type::const_CornerIterator   const_CornerIterator;
   /// The \c mesh_type must provide a \c SweepIterator type.
   typedef typename mesh_type::SweepIterator          SweepIterator;

   // ****** Tests the required mesh functionality ******
   /*! \brief The constraints method tests that the \c mesh_type 
    *         provides certain functionality. */
   void constraints()
   {
      function_requires< DimensionConcept<DimensionType> >();
      function_requires< ConvertibleConcept<LengthType, Real8> >();
      function_requires< UnsignedIntegerConcept<SizeType> >();
   
      // Basic mesh sizes
      LengthType length = mMesh->length();
      LengthType area   = mMesh->area();
      LengthType volume = mMesh->volume();

      // Meshes must have a way to number the geometric elements      
      SizeType num_zones   = mMesh->numZones();
      SizeType num_nodes   = mMesh->numNodes();
      SizeType num_corners = mMesh->numCorners();
      
      // Meshes must have a way to access particular elements
      const Zone& zone     = mMesh->getZone( mZoneId );
      const Node& node     = mMesh->getNode( mNodeId );
      const Corner& corner = mMesh->getCorner( mCornerId );
      
      // Meshes must support at least forward iterators over each element
      mZoneIterator   = mMesh.zoneBegin();
      mZoneIterator   = mMesh.zoneEnd();
      mNodeIterator   = mMesh.nodeBegin();
      mNodeIterator   = mMesh.nodeEnd();
      mCornerIterator = mMesh.cornerBegin();
      mCornerIterator = mMesh.cornerEnd();
      
      // Meshes must support sweep iterators
      mSweepIterator = mMesh.sweepBegin(mAngle);
      mSweepIterator = mMesh.sweepEnd(mAngle);
   }
   
private:
   /// The \c mesh_type to use for the tests.
   mesh_type            mMesh;
   /// Used to test that the \c length() method returns a \c LengthType.
   LengthType           mLength;
   /// Used to test that the \c area() method returns a \c LengthType.
   LengthType           mArea;
   /// Used to test that the \c volume() method returns a \c LengthType.
   LengthType           mVolume;
   /// Used to test that the \c numZones() method returns a \c SizeType.
   SizeType             mNumZones;
   /// Used to test that the \c numNodes() method returns a \c SizeType.
   SizeType             mNumNodes;
   /// Used to test that the \c numCorners() method returns a \c SizeType.
   SizeType             mNumCorners;
   /// Used to test the \c getZone(Zone::Id) method.
   typename Zone::Id    mZoneId;
   /// Used to test the \c getNode(Node::Id) method.
   typename Node::Id    mNodeId;
   /// Used to test the \c getCorner(Corner::Id) method.
   typename Corner::Id  mCornerId;
   /*! \brief Used to test that the \c zoneBegin() and \c zoneEnd() methods
    *         return a \c const_ZoneIterator. */
   const_ZoneIterator   mZoneIterator;
   /*! \brief Used to test that the \c nodeBegin() and \c nodeEnd() methods
    *         return a \c const_NodeIterator. */
   const_NodeIterator   mNodeIterator;
   /*! \brief Used to test that the \c cornerBegin() and \c cornerEnd() methods
    *         return a \c const_CornerIterator. */
   const_CornerIterator mCornerIterator;
   /*! \brief Used to test that the \c sweepBegin(angle) and \c sweepEnd(angle)
    *         methods return a \c SweepIterator. */
   SweepIterator        mSweepIterator;
   /// Used as a parameter for the \c sweepBegin(angle) and \c sweepEnd(angle) methods.
   Angle<DimensionType> mAngle;
};

#endif

