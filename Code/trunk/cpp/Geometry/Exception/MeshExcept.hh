// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file MeshExcept.hh
 *  \author Greg Davidson
 *  \date June 3, 2005
 * 
 *  \brief Provides all of the exceptions required by the Mesh Module.
 * 
 *  This file provides the exceptions required by the Mesh Module, which includes
 *  the Mesh class as well as the geometric elements \c Node, \c Zone, and \c Corner, 
 *  as well as supporting classes.  The exception classes all derive from ExceptionBase. */
 
#ifndef MESHEXCEPT_HH
#define MESHEXCEPT_HH

#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>
#include <boost/call_traits.hpp>

#include "ExceptionBase.hh"
#include "Dimension.hh"
#include "MeshTraits.hh"
#include "MeshExceptStrings.hh"
#include "GeometricElementConcept.hh"

using boost::lexical_cast;
using boost::call_traits;


/*! \addtogroup ExceptMod Exception Module
 *  @{  */


/*! \brief Base exception class for all \c CartesianMesh exceptions.
 *
 *  This is a base exception class for all of the \c CartesianMesh exceptions.  
 *  It adds no functionality to the \c ExceptionBase class. */
class MeshBaseException : public ExceptionBase
{
public:
   /// Imports the \c LineNumType from the \c ExceptionBase base class.
	typedef ExceptionBase::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   MeshBaseException(LineNumType line_number, const char* file_in)
      : ExceptionBase(line_number, file_in)
   {  }
};

/*! \brief Thrown when a non-existent connected element access is attempted.
 *
 *  This class is thrown when some geometric element attempts to access through
 *  its connectivity another geometric element that does not actually exist, i.e.
 *  if a \c Node on the left boundary attempts to access its left \c Zone object,
 *  which obviously would not exist.
 *  \par Template Parameters:
 *     <dl> <dt> \e source_element </dt>
 *          <dd> The type of the geometric element that made the bad call. </dd>
 *          <dt> \e target_element </dt>
 *          <dd> The type of geometric element whose access was attempted. </dd> </dl> */
template<typename source_element, typename target_element>
class NoElementConnected : public MeshBaseException
{
private:
   /// Requires that the \c source_element type satisfy the \c GeometricElementConcept concept.
   BOOST_CLASS_REQUIRE(source_element, , GeometricElementConcept);

   /// Defines the \c SourceIdType from the \c source_element::Id type.
   typedef typename source_element::Id          SourceIdType;
   /// Defines the \c SourceIndexType from the \c source_element::IndexType type.
   typedef typename source_element::IndexType   SourceIndexType;
public:
   /// Defines the \c SourceElementType from the \c source_element template parameter.
   typedef source_element     SourceElementType;
   /// Defines the \c TargetElementType from the \c target_element template parameter.
   typedef target_element     TargetElementType;

   /*! \brief Constructor.  Takes in the line number and file where the exception is thrown,
    *         and the source id number and local target id number.
    *
    *  Construtor.  Takes in the line number and filename where the exception is thrown. Also
    *  takes in the id number of the \c SourceElement and the local target id number. 
    *  \param[in]  line_number          The line number where the exception was thrown.
    *  \param[in]  file_in              The name of the file where the exception was thrown.
    *  \param[in]  source_id_number     The id number of the source element.
    *  \param[in]  local_target_number  The local index number for the target element. */
   NoElementConnected(MeshBaseException::LineNumType line_number,
                      const char* file_in,
                      typename call_traits<SourceIdType>::param_type source_id_number,
                      typename call_traits<SourceIndexType>::param_type local_target_number)
      : MeshBaseException(line_number, file_in),
        mSourceId(source_id_number),
        mLocalTargetId(local_target_number)
   {  }
   
   /*! \brief Returns the source id number.
    *
    *  Returns the source id number.
    *  \return  The source id number. */
   typename call_traits<typename SourceElementType::Id>::const_reference 
   getSourceId() const 
   { 
      return mSourceId; 
   }

   /*! \brief Returns the local target id number.
    *
    *  Returns the local target id number.
    *  \return  The local target id number. */
   typename call_traits<typename SourceElementType::IndexType>::const_reference 
   getLocalTargetId() const 
   { 
      return mLocalTargetId; 
   }
   
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const
   {
      return geomElementString<SourceElementType>() + " number "
             + lexical_cast<string>( mSourceId.idNumber() )
             + " attempted to reference a non-existent "
             + geomElementString<TargetElementType>()
             + " with local id number "
             + lexical_cast<string>(mLocalTargetId);
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */   
   virtual string what() const 
   {
      return "NoElementConnected< "
                + geomElementString<SourceElementType>()
                + ", " + geomElementString<TargetElementType>() + " >";
   }
      
private:
   /// Stores the id number of the source element.
   typename SourceElementType::Id mSourceId;
   /// Stores the local target id.
   typename SourceElementType::IndexType mLocalTargetId;
};

/*! \brief Thrown when a non-existent element access is attempted through the 
 *         a \c CartesianMesh object.
 *
 *  This object is thrown when an attempt is made to access a non-existent geometric
 *  element through a \c CartesianMesh object.
 *  \par Template Parameters:
 *     <dl> <dt> \e geom_element </dt>
 *          <dd> The type of geometric element whose access was attempted. </dd> </dl> */
template<class geom_element>
class ElementDoesNotExist : public MeshBaseException
{
private:
   /// Requires the \c geom_element type satisfies the \c GeometricElementConcept concept.
   BOOST_CLASS_REQUIRE(geom_element, , GeometricElementConcept);
   /// Defines the \c GeomElementType from the template parameter \c geom_element.
   typedef geom_element   GeomElementType;
   
public:
	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown and the element id.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown,
    *  and the id number of the attempted element access.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. 
    *  \param[in] id_in        The id number given to the \c CartesianMesh to attempt the
    *                          geometric element access. */
   ElementDoesNotExist(MeshBaseException::LineNumType line_number,
                       const char* filename,
                       typename call_traits<typename GeomElementType::Id>::param_type id_in)
      : MeshBaseException(line_number, filename),
        mId(id_in)
   {  }
   
   /*! \brief Returns the id of the attempted element access.
    *
    *  Returns the id that was passed to the \c CartesianMesh in the attempted element
    *  access
    *  \return  The id that was passed to the \c CartesianMesh in the attempted element
    *           access. */
   typename call_traits<typename GeomElementType::Id>::const_reference getId() const 
   { 
      return mId; 
   }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */   
   virtual string msg() const
   {
      return "Attempted to access a non-existent "
             + geomElementString<GeomElementType>()
             + " with Id number "
             + lexical_cast<string>( mId.idNumber() );
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */      
   virtual string what() const 
   {
      return "ElementDoesNotExist< "
             + geomElementString<GeomElementType>() + " >";
   }
   
private:
   /// Stores the id of the attempted element access.
   const typename GeomElementType::Id& mId;
};

/*! \brief Base class for the exceptions that check the \c CartesianMesh input data.
 *
 *  This is a base class for the exceptions that are thrown when a \c CartesianMesh
 *  object is passed bad data. */
class BadMeshUserData : public MeshBaseException
{
public:
	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   BadMeshUserData(MeshBaseException::LineNumType line_number,
                   const char* filename)
      : MeshBaseException(line_number, filename)
   {  }
};

/*! \brief Thrown when a \c CartesianMesh object is given a bad length.
 *
 *  This exception is thrown when a \c CartesianMesh is given an invalid length.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c CartesianMesh spans.  This type must
 *               satisfy the \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class BadMeshLength : public BadMeshUserData
{
private:
   /// Requires the \c dimension_type template parameter satisfies the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);
   /// Defines the \c DimensionType type from the \c dimension_type template parameter.
   typedef dimension_type    DimensionType;
   
public:
   /// Defines the \c LengthType from the \c MeshTraits<DimensionType>::LengthType type.
   typedef typename MeshTraits<DimensionType>::LengthType  LengthType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown and the mesh length.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown,
    *  and the length given to the mesh constructor.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. 
    *  \param[in] mesh_length  The length given to the mesh constructor. */  
   BadMeshLength(MeshBaseException::LineNumType line_number,
                 const char* filename,
                 LengthType mesh_length)
      : BadMeshUserData(line_number, filename),
        mMeshLength(mesh_length)
   {  }
   
   /*! \brief Returns the length given to the \c CartesianMesh object.
    *
    *  Returns the length given to the \c CartesianMesh object. 
    *  \return The length given to the \c CartesianMesh object. */
   LengthType getMeshLength() const 
   { 
      return mMeshLength; 
   }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */       
   virtual string msg() const
   {
      return "Mesh length is not positive.  Mesh length is "
             + lexical_cast<string>(mMeshLength);
   }
   
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */          
   virtual string what() const 
   { 
      return "BadMeshLength< " 
                  + dimensionString<DimensionType>() + " >";
   }
               
private:
   /// Stores the length given to the \c CartesianMesh object.
   LengthType mMeshLength;
};

/*! \brief Thrown when a \c CartesianMesh object is given a bad area.
 *
 *  This exception is thrown when a \c CartesianMesh is given an invalid area.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c CartesianMesh spans.  This type
 *               must satisfy the \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class BadMeshArea : public BadMeshUserData
{
private:
   /// Requires the \c dimension_type template parameter satisfies the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);
   /// Defines the \c DimensionType type from the \c dimension_type template parameter.
   typedef dimension_type    DimensionType;
   
public:
   /// Defines the \c LengthType from the \c MeshTraits<DimensionType>::LengthType type.
   typedef typename MeshTraits<DimensionType>::LengthType  LengthType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown and the mesh area.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown,
    *  and the area given to the mesh constructor.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. 
    *  \param[in] mesh_area    The area given to the mesh constructor. */     
   BadMeshArea(MeshBaseException::LineNumType line_number,
               const char* filename,
               LengthType mesh_area)
      : BadMeshUserData(line_number, filename),
        mArea(mesh_area)
   {  }

   /*! \brief Returns the area given to the \c CartesianMesh object.
    *
    *  Returns the area given to the \c CartesianMesh object. 
    *  \return The area given to the \c CartesianMesh object. */   
   LengthType getMeshArea() const 
   { 
      return mArea; 
   }
   
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */          
   virtual string msg() const
   {
      return "Mesh area is not positive.  Mesh area is "
             + lexical_cast<string>(mArea);
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */          
   virtual string what() const 
   { 
      return "BadMeshArea< "
                  + dimensionString<DimensionType>() + " >";
   }

private:
   /// Stores the area given to the \c CartesianMesh object.
   LengthType mArea;
};

/*! \brief Thrown when a \c CartesianMesh object has zero zones.
 *
 *  Thrown when a \c CartesianMesh object has zero zones. */
class ZeroZones : public BadMeshUserData
{
public:
	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown. 
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   ZeroZones(MeshBaseException::LineNumType line_number,
             const char* filename)
      : BadMeshUserData(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */             
   virtual string msg() const
   {
      return "Mesh has zero zones!";
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */             
   virtual string what() const 
   {
      return "ZeroZones";
   }
};

/*! \brief Thrown when one of the zones has a negative length.
 *
 *  This exception class is thrown when one of the lengths in the list of
 *  \c Zone lengths is negative.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c CartesianMesh spans.  This type
 *               must satisfy the \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class NegativeZoneLength : public BadMeshUserData
{
private:
   /// Requires the \c dimension_type template parameter satisfies the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);
   /// Defines the \c DimensionType type from the \c dimension_type template parameter.
   typedef dimension_type     DimensionType;
public:
   /// Defines the \c LengthType from the \c MeshTraits<DimensionType>::LengthType type.
   typedef typename MeshTraits<DimensionType>::LengthType            LengthType;
   /// Defines the \c IdType from the \c MeshTraits<DimensionType>::Zone::Id::IdType type.   
   typedef typename MeshTraits<DimensionType>::Zone::Id::IdType      IdType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown, the id of the negative zone, and the negative length.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown,
    *  as well as the id number of the \c Zone with the negative length, and the length itself. 
    *  \param[in] line_number    The line number where the exception was thrown.
    *  \param[in] filename       The filename where the exception was thrown. 
    *  \param[in] negative_zone  The id of the zone with the negative length.
    *  \param[in] zone_length    The length of the negative-lengthed \c Zone. */
   NegativeZoneLength(typename MeshBaseException::LineNumType line_number,
                      const char* filename,
                      typename call_traits<IdType>::param_type negative_zone,
                      LengthType zone_length)
      : BadMeshUserData(line_number, filename),
        mNegativeZone(negative_zone),
        mNegativeZoneLength(zone_length)
   {  }
   
   /*! \brief Returns the id number of the \c Zone with the negative length.
    * 
    *  Returns the id number of the \c Zone with the negative length.
    *  \return  The id number of the \c Zone with the negative length. */
   typename call_traits<IdType>::const_reference   
   getBadZoneId() const     
   { 
      return mNegativeZone; 
   }
   
   /*! \brief The length of the \c Zone that is negative.
    *
    *  The length of the \c Zone that is negative.
    *  \return The length of the \c Zone that is negative. */
   LengthType getBadZoneLength() const 
   { 
      return mNegativeZoneLength; 
   }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */                
   virtual string msg() const
   {
      return "Zone< " + dimensionString<DimensionType>() + " > with id number "
             + lexical_cast<string>(mNegativeZone)
             + " has a non-positive length of "
             + lexical_cast<string>(mNegativeZoneLength);
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */                
   virtual string what() const 
   {
      return "NegativeZoneLength< " 
                     + dimensionString<DimensionType>() + " >";
   }
   
private:
   /// Stores the id number of the \c Zone with a negative length.
   IdType  mNegativeZone;
   /// Stores the negative length.
   LengthType mNegativeZoneLength;
};

///  @}

#endif

