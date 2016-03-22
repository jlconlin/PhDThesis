// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file GeometricElementConcept.hh
 *  \author Greg Davidson
 *  \date November 6, 2006
 *  
 *  \brief Provides various geometric concept classes.
 * 
 *  This file provides the \c GeometricElementConcept, \c ZoneConcept, \c NodeConcept,
 *  and \c CornerConcept classes.  These classes describe the functionality that geometric
 *  elements, <tt>Zone</tt>s, <tt>Node</tt>s, and <tt>Corner</tt>s must implement, respectively. */

#ifndef GEOMETRICELEMENTCONCEPT_HH
#define GEOMETRICELEMENTCONCEPT_HH

#include <ostream>

#include <boost/concept_check.hpp>

#include "Types.hh"

using std::ostream;
using boost::function_requires;
using boost::UnsignedIntegerConcept;
using boost::ConvertibleConcept;

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/// Forward declaration of the \c CartesianMesh class.
template<typename dimension_type> class CartesianMesh;

/// Forward declaration of the \c numCentering function.
template<typename dimension_type, typename geom_elem>
inline typename CartesianMesh<dimension_type>::SizeType 
numCentering(const CartesianMesh<dimension_type>& mesh);


/*! \brief The \c GeometricElementConcept class depicts the functionality that all geometric 
 *         types must provide.
 *
 *  The purpose of the \c GeometricElementConcept template class depicts the functionality 
 *  that all geometric types must provide.
 *  \par Template Parameters:
 *     <dl> <dt> \e geom_elem </dt>
 *          <dd> This is the type to test against the \c GeometricElementConcept. </dd> </dl>
 *  \par Concept Requirements:
 *       The following types must be provided by the \c geom_elem type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c DimensionType </TD> 
 *                <TD> The \c geom_elem type must provide a \c DimensionType. </TD> </TR>
 *          <TR>  <TD> \arg \c Id </TD> 
 *                 <TD> All \c geom_elem types must provide an \c Id. </TD> </TR>
 *          <TR>  <TD> \arg \c Id::IdType </TD> 
 *                <TD> The \c Id type must be valid and provide an \c IdType. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c geom_elem type:
 *       \arg The \c geom_elem must provide an \c id() method.
 *       \arg The \c geom_elem must provide a \c printId(ostream&) method.
 *       \arg A partial template specialization of \c numCentering() must exist for
 *            this \c geom_elem type.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename geom_elem>
class GeometricElementConcept
{
public:
   // ****** Defines the basic GeometricElement types ******
   /// Alias the \c geom_elem type parameter as a \c GeomElem.
   typedef geom_elem                         GeomElem;
   /// The \c GeomElem type must provide a \c DimensionType.
   typedef typename GeomElem::DimensionType  DimensionType;
   /// The \c GeomElem type must provide a \c Id type.
   typedef typename GeomElem::Id             Id;
   /// The \c Id type must be a valid \c Id type with a \c IdType.
   typedef typename GeomElem::Id::IdType     IdType;
   
   // ****** Tests the GeometricElement functionality ******
   /*! \brief The constraints method tests that the \c geom_elem 
    *         provides certain functionality. */
   void constraints()
   {
      function_requires< UnsignedIntegerConcept<IdType> >();
      
      mId = mGeomElem->id();
      mGeomElem->printId(mOutStream);
      numCentering<DimensionType, GeomElem>(*mMesh);   
   }
   
private:
   /// A \c CartesianMesh pointer type for calling the \c numCentering() function.
   CartesianMesh<DimensionType>* mMesh;
   /// A \c GeomElem pointer for testing the element's functionality.
   GeomElem*   mGeomElem;
   /// An \c Id type for testing that \c mGeomElem->id() returns an \c Id type.
   Id mId;
   /// An \c ostream type for testing the \c printId(ostream&) method.
   ostream mOutStream;
};

/*! \brief The \c ZoneConcept concept class is used to test whether a type is a valid 
 *         \c Zone.
 *
 *  The \c ZoneConcept concept class is used to test whether a particular type offers the
 *  functionality of a valid \c Zone.
 *  \par Template Parameters:
 *     <dl> <dt> \e zone_type </dt>
 *          <dd> This is the type to test against the \c ZoneConcept. </dd> </dl>
 *  \par Concept Requirements
 *       The following types must be provided by the \c zone_elem type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c LengthType </TD> 
 *                <TD> The \c zone_type type must provide a \c LengthType. </TD> </TR>
 *          <TR>  <TD> \arg \c SizeType </TD> 
 *                <TD> The \c zone_type type must provide a \c SizeType. </TD> </TR>
 *          <TR>  <TD> \arg \c IndexType </TD> 
 *                <TD> The \c zone_type type must provide an \c IndexType. </TD> </TR>
 *          <TR>  <TD> \arg \c Id </TD> 
 *                <TD> The \c zone_type type must provide an \c Id. </TD> </TR>
 *          <TR>  <TD> \arg \c Node </TD> 
 *                <TD> The \c zone_type type must provide a \c Node for 
 *                                                                  connectivity. </TD> </TR>
 *          <TR>  <TD> \arg \c Corner </TD> 
 *                <TD> The \c zone_type type must provide a \c Corner for 
 *                                                                  connectivity. </TD> </TR>
 *          <TR>  <TD> \arg \c PointType </TD> 
 *                <TD> The \c zone_type type must provide a \c PointType. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c zone_elem type:
 *       \arg The \c zone_type must satisfy the \c GeometricElementConcept concept.
 *       \arg The \c length_type must be convertible to the \c Real8 type.
 *       \arg The \c SizeType must satisfy the \c UnsignedIntegerConcept type.
 *       \arg The \c IndexType must satisfy the \c UnsignedIntegerConcept type.
 *       \arg The \c zone_type must provide an \c id() method.
 *       \arg The \c zone_type must provide a \c length() method.
 *       \arg The \c zone_type must provide an \c area() method.
 *       \arg The \c zone_type must provide a \c volume() method.
 *       \arg The \c zone_type must provide a \c getNode(IndexType) method.
 *       \arg The \c zone_type must provide a \c getCorner(IndexType) method.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename zone_type>
class ZoneConcept
{
public:
   // ****** Defines the basic Zone types ******
   /// The \c zone_type must provide a \c LengthType.
   typedef typename zone_type::LengthType    LengthType;
   /// The \c zone_type must provide a \c SizeType.
   typedef typename zone_type::SizeType      SizeType;
   /// The \c zone_type must provide an \c IndexType.
   typedef typename zone_type::IndexType     IndexType;
   /// The \c zone_type must provide an \c Id type. 
   typedef typename zone_type::Id            Id;
   /// The \c zone_type must provide a \c Node for connectivity.
   typedef typename zone_type::Node          Node;
   /// The \c zone_type must provide a \c Corner for connectivity.
   typedef typename zone_type::Corner        Corner;
   /// The \c zone_type must provide a \c PointType.
   typedef typename zone_type::PointType     PointType;

   // ****** Tests the basic Zone functionality ******
   /*! \brief The constraints method tests that the \c zone_type 
    *         provides certain functionality. */   
   void constraints()
   {
      function_requires< GeometricElementConcept<zone_type> >();
      function_requires< ConvertibleConcept<LengthType, Real8> >();
      function_requires< UnsignedIntegerConcept<SizeType> >();
      function_requires< UnsignedIntegerConcept<IndexType> >();
   
      Id mId    = mZone.id();
      mLength   = mZone.length();
      mArea     = mZone.area();
      mVolume   = mZone.volume();
      mCenter   = mZone.center();
      mZone.getNode(mIndex);
      mZone.getCorner(mIndex);
   }
   
private:
   /// The \c zone_type to test.
   zone_type mZone;
   /// Used to check that the \c id() method returns an \c Id type.
   Id        mId;
   /// Used to test that the \c length() method returns a \c LengthType.
   LengthType mLength;
   /// Used to test that the \c area() method returns a \c LengthType.
   LengthType mArea;
   /// Used to test that the \c volume() method returns a \c LengthType.
   LengthType mVolume;
   /// Used to test that the \c center() method returns a \c PointType.
   PointType  mCenter;
   /*! \brief Used to test the \c getNode(IndexType) and 
    *         \c getCorner(IndexType) methods. */
   IndexType mIndex;
};

/*! \brief The \c NodeConcept concept class is used to test whether a type is a valid 
 *         \c Node.
 *
 *  The \c NodeConcept concept class is used to test whether a particular type offers the
 *  functionality of a valid \c Node.
 *  \par Template Parameters:
 *     <dl> <dt> \e node_type </dt>
 *          <dd> This is the type to test against the \c NodeConcept. </dd> </dl>
 *  \par Concept Requirements:
 *       The following types must be provided by the \c node_elem type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c Id </TD> 
 *                <TD> The \c node_type type must provide an \c Id. </TD> </TR>
 *          <TR>  <TD> \arg \c PointType </TD> 
 *                <TD> The \c zone_type type must provide a \c PointType. </TD> </TR>
 *          <TR>  <TD> \arg \c IndexType </TD> 
 *                <TD> The \c zone_type type must provide an \c IndexType. </TD> </TR>
 *          <TR>  <TD> \arg \c Zone </TD> 
 *                <TD> The \c node_type type must provide a \c Zone for
 *                                                                 connectivity. </TD> </TR>
 *          <TR>  <TD> \arg \c Corner </TD> 
 *                <TD> The \c node_type type must provide a \c Corner for 
 *                                                                 connectivity. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c node_elem type:
 *       \arg The \c node_type must satisfy the \c GeometricElementConcept concept.
 *       \arg The \c length_type must be convertible to the \c Real8 type.
 *       \arg The \c IndexType must satisfy the \c UnsignedIntegerConcept type.
 *       \arg The \c node_type must provide an \c id() method.
 *       \arg The \c node_type must provide an \c onBoundary() method.
 *       \arg The \c node_type must provide a \c position() method.
 *       \arg The \c node_type must provide a \c x() method.
 *       \arg The \c node_type must provide a \c getZone(IndexType) method.
 *       \arg The \c node_type must provide a \c getCorner(IndexType) method.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename node_type>
class NodeConcept
{
private:
   /// We require that the \c Node \c PointType provide a \c PointType.
   typedef typename node_type::PointType::PointType   XType;
public:
   // ****** Defines the basic Node functionality ******
   /// The \c node_type is required to provide an \c Id type.
   typedef typename node_type::Id         Id;
   /// The \c node_type is required to provide a \c PointType type.
   typedef typename node_type::PointType  PointType;
   /// The \c node_type is required to provide a \c IndexType type.
   typedef typename node_type::IndexType  IndexType;
   /// The \c node_type is required to provide a \c Zone type.
   typedef typename node_type::Zone       Zone;
   /// The \c node_type is required to provide a \c Corner type.
   typedef typename node_type::Corner     Corner;
   
   // ****** Tests the basic Node functionality ******
   /*! \brief The constraints method tests that the \c node_type 
    *         provides certain functionality. */      
   void constraints()
   {
      function_requires< GeometricElementConcept<node_type> >();
      function_requires< ConvertibleConcept<XType, Real8> >(); 
      function_requires< UnsignedIntegerConcept<IndexType> >();
      
      mId = mNode.id();
      mOnBoundary = mNode.onBoundary();
      mPosition   = mNode.position();
      mX  = mNode.x();
      mNode.getZone(mIndex);
      mNode.getCorner(mIndex);
   }
   
private:
   /// The \c node_type to test.
   node_type mNode;
   /// Used to check that the \c id() method returns an \c id() type.
   Id mId;
   /// Used to check that the \c onBoundary() method returns a boolean.
   bool mOnBoundary;
   /// Used to check that the \c position() method returns a \c PointType type.
   PointType mPosition;
   /// Used to check that the \c x() method returns a \c PointType::PointType type.
   XType mX;
   /// Used to test the \c getZone(IndexType) and \c getCorner(IndexType) methods.
   IndexType mIndex;
};

/*! \brief The \c CornerConcept concept class is used to test whether a type is a valid 
 *         \c Corner.
 *
 *  The \c CornerConcept concept class is used to test whether a particular type offers the
 *  functionality of a valid \c Corner type.
 *  \par Template Parameters:
 *     <dl> <dt> \e corner_type </dt>
 *          <dd> This is the type to test against the \c CornerConcept. </dd> </dl>
 *  \par Concept Requirements:
 *       The following types must be provided by the \c node_elem type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c Id </TD> 
 *                <TD> The \c corner_type type must provide an \c Id type. </TD> </TR>
 *          <TR>  <TD> \arg \c Zone </TD> 
 *                <TD> The \c corner_type type must provide a \c Zone type for
 *                                                                 connectivity. </TD> </TR>
 *          <TR>  <TD> \arg \c Node </TD> 
 *                <TD> The \c corner_type type must provide a \c Node type for 
 *                                                                 connectivity. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c corner_elem type:
 *       \arg The \c corner_type must satisfy the \c GeometricElementConcept concept.
 *       \arg The \c corner_type must provide an \c id() method.
 *       \arg The \c corner_type must provide a \c getZone(IndexType) method.
 *       \arg The \c corner_type must provide a \c getNode(IndexType) method.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename corner_type>
class CornerConcept
{
public:
   // ****** Defines the basic Corner types ******
   /// The \c corner_type must provide an \c Id type.
   typedef typename corner_type::Id       Id;
   /// The \c corner_type must provide a \c Zone type.
   typedef typename corner_type::Zone     Zone;
   /// The \c corner_type must provide a \c Node type.
   typedef typename corner_type::Node     Node;

   // ****** Tests the basic Corner functionality ******
   /*! \brief The constraints method tests that the \c corner_type 
    *         provides certain functionality. */         
   void constraints()
   {
      function_requires< GeometricElementConcept<corner_type> >();
      
      mId = mCorner.id();
      mCorner.getZone();
      mCorner.getNode();
   }
   
private:
   /// The \c corner_type we wish to test.
   corner_type mCorner;
   /// Used to ensure that the \c id() method returns the \c Id type.
   Id mId;
};


/// @}
#endif

