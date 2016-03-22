// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file FieldExcept.hh
 *  \author Greg Davidson
 *  \date July 21, 2008
 * 
 *  \brief Provides all of the exceptions required by the Field Module.
 * 
 *  This file provides the exceptions required by the Field Module. */
 
#ifndef FIELDEXCEPT_HH
#define FIELDEXCEPT_HH

#include <boost/concept_check.hpp>

#include "ExceptionBase.hh"
#include "FieldConcept.hh"

/*! \addtogroup FieldMod Field Module
 *  @{  */

/// Forward declaration of the \c Field class. */
template< typename geom_elem, typename data_type, typename container_type  >
class Field;

/*! \brief Base class for all of the \c Field exceptions.
 *
 *  This class provides a base class for all exceptions thrown by the \c Field module.
 *  \par Template Parameters:
 *     <dl> <dt> \e field_type </dt>
 *          <dd> This is the type of the \c Field object from which the exception 
 *               was thrown. </dd> </dl> */
template<typename field_type>
class FieldBaseException : public ExceptionBase
{
private:
   /// Requires that the \c field_type satisfy the \c FieldConcept concept.
   BOOST_CLASS_REQUIRE(field_type, , FieldConcept);   
   
public:
   /// Imports the \c LineNumType from \c ExceptionBase.
   typedef ExceptionBase::LineNumType     LineNumType;
   /// Defines the \c field_type template paramater as \c FieldType.
   typedef field_type                     FieldType;
   
   /*! \brief Constructor.  Takes in the \c Field from which the exception was thrown, and the
    *         line number and filename.
    *
    *  Constructor.  Takes in a reference to the \c Field from which the exception was thrown.
    *  Also takes in the line number and the name of the file where the exception was thrown.
    *  \param[in] field_in     A reference to the \c Field from which the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] file_in      The name of the file where the exception was thrown. */
   FieldBaseException(const FieldType& field_in, LineNumType line_number,
                      const char* file_in)
      : ExceptionBase(line_number, file_in),
        mField(field_in)
   {  }

   /*! \brief Returns a reference to the \c Field where the exception was thrown.
    *
    *  Returns a reference to the \c Field where the exception was thrown.
    *  \return  A reference to the \c Field where the exception was thrown. */
   const FieldType& field() const
   {
      return mField;
   }

private:
   /// Holds a reference to the \c Field where the exception was thrown.
   const FieldType& mField;
};   
   
/*! \brief Thrown when \c Field objects are constructed with the wrong size.
 *
 *  This exception class is thrown when \c Field objects are constructed with a
 *  container object that contains the incorrect number of elements for the \c Field,
 *  i.e. there is a different number of geometric elements in the \c CartesianMesh than
 *  there was elements in the data container.
 *  \par Template Parameters:
 *     <dl> <dt> \e field_type </dt>
 *          <dd> This is the type of the \c Field object from which the exception 
 *               was thrown. </dd> </dl> */   
template<typename field_type>
class WrongSizedField : public FieldBaseException<field_type>
{
private:
   /// Requires that the \c field_type satisfy the \c FieldConcept concept.
   BOOST_CLASS_REQUIRE(field_type, , FieldConcept); 

public:
   /// Imports the \c LineNumType from \c ExceptionBase.
   typedef typename FieldBaseException<field_type>::LineNumType  LineNumType;
   /// Defines the \c field_type template paramater as \c FieldType.
   typedef field_type                                            FieldType;
   /// Defines the type of the \c CartesianMesh that corresponds to this \c Field.
   typedef typename FieldType::MeshType                          MeshType;

   /*! \brief Constructor.  Takes in the \c Field from which the exception was thrown, the
    *         \c CartesianMesh and the line number and filename.
    *
    *  Constructor.  Takes in the \c CartesianMesh that corresponds to the \c Field from which
    *  this exception was thrown and a reference to the \c Field from which the exception was thrown.
    *  Also takes in the line number and the name of the file where the exception was thrown.
    *  \param[in] mesh_in      The \c CartesianMesh that corresponds to this \c Field.
    *  \param[in] field_in     A reference to the \c Field from which the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] file_in      The name of the file where the exception was thrown. */
   WrongSizedField(const MeshType& mesh_in,
                   const FieldType& field_in, LineNumType line_number,
                   const char* file_in)
      : FieldBaseException<field_type>(field_in, line_number, file_in),
        mMesh(mesh_in)
   {  }
  
   /*! \brief Returns a reference to the \c CartesianMesh that corresponds to this \c Field.
    *
    *  Returns a reference to the \c CartesianMesh that corresponds to this \c Field.
    *  \return A reference to the \c CartesianMesh that corresponds to this \c Field. */
   const typename FieldType::MeshType& mesh() const
   {
      return mMesh;
   }
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const
   {
      return "Field was constructed with a mismatched size!";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */   
   virtual string what() const
   {
      return "WrongSizedField<field_type>";
   }

private:
   /// Stores a reference to the \c CartesianMesh corresponding to the \c Field.
   const MeshType& mMesh;
};

/*! \brief Thrown when access to data outside the <tt>Field</tt>'s range is attempted.
 *
 *  This exception class is thrown when an attempt is made to access data outside the 
 *  <tt>Field</tt>'s range.
 *  \par Template Parameters:
 *     <dl> <dt> \e field_type </dt>
 *          <dd> This is the type of the \c Field object from which the exception 
 *               was thrown. </dd> </dl> */   
template<typename field_type>
class FieldRangeError : public FieldBaseException<field_type>
{
private:
   /// Requires that the \c field_type satisfy the \c FieldConcept concept.
   BOOST_CLASS_REQUIRE(field_type, , FieldConcept); 

public:
   /// Imports the \c LineNumType from \c ExceptionBase.
   typedef typename FieldBaseException<field_type>::LineNumType  LineNumType;
   /// Defines the \c field_type template paramater as \c FieldType.
   typedef field_type                                            FieldType;

   /*! \brief Constructor.  Takes in the \c CenteringType object of the attempted access, the
    *         \c Field and the line number and filename.
    *
    *  Constructor.  Takes in the \c geom_elem used in the failed data access, and the \c Field 
    *  from which this exception was thrown.
    *  Also takes in the line number and the name of the file where the exception was thrown.
    *  \param[in] geom_elem    A reference to the geometric element used in the attempted data
    *                          access.
    *  \param[in] field_in     A reference to the \c Field from which the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] file_in      The name of the file where the exception was thrown. */
   FieldRangeError(const typename FieldType::CenteringType& geom_elem,
                   const FieldType& field_in, LineNumType line_number,
                   const char* file_in)
      : FieldBaseException<field_type>(field_in, line_number, file_in),
        mGeomElem(geom_elem)
   {  }

   /*! \brief  Returns a reference to the geometric element used in the attempted data access.
    *
    *  Returns a reference to the geometric element used in the attempted data access.
    *  \return A reference to the geometric element used in the attempted data access. */
   const typename FieldType::CenteringType& centeringObject() const
   {
      return mGeomElem;
   }
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */   
   virtual string msg() const
   {
      return "Attempted to access a field beyond its range!";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */      
   virtual string what() const
   {
      return "FieldRangeError<field_type>";
   }
   
private:
   /// Stores the reference to the geometric element used in the attempted data access.
   const typename FieldType::CenteringType& mGeomElem;
};

/// @}

#endif


