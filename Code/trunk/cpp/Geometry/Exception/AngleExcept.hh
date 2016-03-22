// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file AngleExcept.hh
 *  \author Greg Davidson
 *  \date July 23, 2008
 * 
 *  \brief Provides all of the exceptions required by the Angular Quadrature Module.
 * 
 *  This file provides the exceptions required by the Angular Quadrature Module. */

#ifndef ANGLEEXCEPT_HH
#define ANGLEEXCEPT_HH

#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>

#include "ExceptionBase.hh"
#include "AngleQuadConcept.hh"
#include "AngleVectorConcept.hh"

using boost::lexical_cast;

/*! \addtogroup ExceptMod Exception Module
 *  @{  */

class AngularBaseException : public ExceptionBase
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
   AngularBaseException(LineNumType line_number, const char* file_in)
      : ExceptionBase(line_number, file_in)
   {  }
};


/*! \brief  Thrown when the \c AngleQuad class is constructed with a bad quadrature order.
 *
 *  This exception class is thrown when an \c AngleQuad class is constructed with a bad
 *  quadrature order.
 *  \par Template Parameters:
 *     <dl> <dt> \e angle_quad_type </dt>
 *          <dd> The type of the bad \c AngleQuad object.  This type must satisfy the
 *               \c AngleQuadConcept concept template. </dd> </dl> */
template<typename angle_quad_type>
class BadAngleQuadOrder : public AngularBaseException
{
private:
   /// Requires the \c angle_quad_type to satisfy the \c AngleQuadConcept.
   BOOST_CLASS_REQUIRE(angle_quad_type, , AngleQuadConcept);

public:
   /// Imports the \c LineNumType from the \c AngularBaseException base class.
   typedef AngularBaseException::LineNumType    LineNumType;
   /// Defines the \c AngleQuadType from the \c angle_quad_type template parameter.
   typedef angle_quad_type                      AngleQuadType;
   
   /*! \brief Constructor.  Takes in the bad angular quadrature object and the line number
    *         and filename where the exception was thrown.
    *
    *  Constructor.  Takes in the bad \c AngleQuad angular quadrature object and the
    *  line number and filename where the exception was thrown.
    *  \param[in] angle_quad_in  The \c AngleQuad object where the error occurred.
    *  \param[in] line_number    The line number where the exception was thrown.
    *  \param[in] file_in        The file where the exception was thrown. */
   BadAngleQuadOrder(const AngleQuadType& angle_quad_in,
                     LineNumType line_number, const char* file_in)
         : AngularBaseException(line_number, file_in),
           mAngleQuad(angle_quad_in)
   {  
      function_requires< AngleQuadConcept<AngleQuadType> >();
   }
   
   /*! \brief Returns a reference to the \c AngleQuad object where the error occurred.
    *
    *  Returns a reference to the \c AngleQuad object where the error occurred.
    *  \return A reference to the \c AngleQuad object where the error occurred. */
   const AngleQuadType& getAngleQuad() const
   {
      return mAngleQuad;
   }
   
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const
   {
      return "An angular quadrature set was constructed with a bad quadrature order of "
             + lexical_cast<typename AngleQuadType::SizeType>( mAngleQuad.order() );            
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */   
   virtual string what() const 
   {
      return "BadQuadAngleOrder<angle_quad_type>";
   }
   
private:
   /// Stores a reference to the \c AngleQuad object where the error occurred.
   const AngleQuadType& mAngleQuad;
};


/*! \brief Thrown when an attempt to index out of the range of an \c AngleVector is made.
 *
 *  This exception class is thrown when an attempt is made to access data in an 
 *  \c AngleVector outside that <tt>AngleVector</tt>'s range. 
 *  \par Template Parameters:
 *     <dl> <dt> \e angle_quad_type </dt>
 *          <dd> The type of the \c AngleVector where the error occurred. </dd> </dl> */
template<typename angle_quad_type>
class AngleQuadRangeError : public AngularBaseException
{
private:
   /// Requires that the \c angle_quad_type template parameter satisfies the \c AngleQuadConcept.
   BOOST_CLASS_REQUIRE(angle_quad_type, , AngleQuadConcept);

public:
   /// Imports the \c LineNumType from the \c AngularBaseException base class.
   typedef AngularBaseException::LineNumType    LineNumType;
   /// Defines the \c AngleQuadType from the \c angle_quad_type template parameter.
   typedef angle_quad_type                      AngleQuadType;
   /// Imports the \c AngleQuadType::IndexType as \c IndexType.
   typedef typename AngleQuadType::IndexType    IndexType;

   /*! \brief Constructor.  Takes the angular quadrature object, the offending index, and the
    *         line and file where the exception was thrown.
    *
    *  Constructor.  Takes the angular quadrature object that threw this exception, the index
    *  number that caused the error, and the line number and filename where the exception was
    *  thrown.
    *  \param[in] angle_quad_in  The angle quadrature object that threw this exception.
    *  \param[in] bad_index_in   The angular ordinate index that caused the error.
    *  \param[in] line_number    The line number where the exception was thrown.
    *  \param[in] file_in        The filename where the exception was thrown. */
   AngleQuadRangeError(const AngleQuadType& angle_quad_in,
                       IndexType bad_index_in,
                       LineNumType line_number, const char* file_in) 
         : AngularBaseException(line_number, file_in),
           mAngleQuad(angle_quad_in),
           mBadIndex(bad_index_in)
   {  }
   
   /*! \brief Returns a reference to the \c AngleQuad object where the error occurred.
    *
    *  Returns a reference to the \c AngleQuad object where the error occurred.
    *  \return A reference to the \c AngleQuad object where the error occurred. */
   const AngleQuadType& getAngleQuad() const
   {
      return mAngleQuad;
   }
   /*! \brief Returns the bad index number that prompted the exception.
    *
    *  Returns the bad index number that prompted the error that caused this exception
    *  to be thrown.
    *  \return  The bad index number that prompted the error that caused this exception
    *           to be thrown. */
   IndexType getIndex() const
   {
      return mBadIndex;
   }
   
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const
   {
      return "An attempt was made to access a non-existent angular ordinate at index "
             + lexical_cast<IndexType>(mBadIndex);            
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */   
   virtual string what() const 
   {
      return "AngleQuadRangeError<angle_quad_type>";
   }
   
private:
   /// Stores a reference to the \c AngleQuad object where the error occurred.
   const AngleQuadType& mAngleQuad;
   /// Stores the index number that caused the exception to be thrown.
   IndexType mBadIndex;
};

/*! \brief Thrown when an attempt to index out of the range of an \c AngleVector is made.
 *
 *  This exception class is thrown when an attempt is made to access data in an 
 *  \c AngleVector outside that <tt>AngleVector</tt>'s range. 
 *  \par Template Parameters:
 *     <dl> <dt> \e angle_quad_type </dt>
 *          <dd> The type of the \c AngleVector where the error occurred. </dd> </dl> */
template<typename angle_vector_type>
class AVAccessOutOfRange : public AngularBaseException
{
private:
   BOOST_CLASS_REQUIRE(angle_vector_type, , AngleVectorConcept);

public:
   typedef angle_vector_type                       AngleVectorType;
   typedef typename angle_vector_type::IndexType   IndexType;

   AVAccessOutOfRange(const angle_vector_type& av_in,
                      const IndexType& bad_iter_in,
                      LineNumType line_number, const char* file_in)
         : AngularBaseException(line_number, file_in),
           mAngleVector(av_in),
           mBadIter(bad_iter_in)
   {  }
   
   
   const AngleVectorType& getAngleVector() const
   {
      return mAngleVector;
   }
   
   const IndexType& getIterator() const
   {
      return mBadIter;
   } 

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const
   {
      return "An attempt was made to access an out-of-range element";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */   
   virtual string what() const 
   {
      return "AVAccessOutOfRange<angle_vector_type>";
   }
         
private:
   const AngleVectorType& mAngleVector;
   const IndexType& mBadIter;
};

                       
template<typename angle_vector_type>
class NegativeAngleVectorLength : public AngularBaseException
{
private:
   BOOST_CLASS_REQUIRE(angle_vector_type, , AngleVectorConcept);
   
public:
   typedef angle_vector_type                      AngleVectorType;
   typedef typename angle_vector_type::IndexType  IndexType;
   
   NegativeAngleVectorLength(const AngleVectorType& av_in,
                             const typename IndexType::difference_type length_in,
                             LineNumType line_number, const char* file_in)                             
         : AngularBaseException(line_number, file_in),
           mAngleVector(av_in),
           mLength(length_in)
   {  }
   
   const AngleVectorType& getAngleVector() const
   {
      return mAngleVector;
   }
   
   typename IndexType::difference_type length() const
   {
      return mLength;
   } 

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const
   {
      return "An attempt was made to access an out-of-range element";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */   
   virtual string what() const 
   {
      return "AVAccessOutOfRange<angle_vector_type>";
   }
   
private:
   const AngleVectorType&        mAngleVector;
   typename IndexType::difference_type    mLength;
};



/// @}

#endif
    
    
