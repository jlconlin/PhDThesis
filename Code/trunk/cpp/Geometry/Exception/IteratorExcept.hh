// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file IteratorExcept.hh
 *  \author Greg Davidson
 *  \date February 6, 2005
 *  
 *  \brief Provides the exception classes used by the \c Iterator and \c ReverseIterator
 *         classes.
 * 
 *  This file provides all of the exception classes used by the \c Iterator and
 *  \c ReverseIterator classes.  Mostly, these iterators are thrown by the range-checking
 *  functionality of the \c Iterator class.  The range-checking can be turned off by
 *  defining the \b NITERCHK macro. */

#ifndef ITERATOREXCEPT_HH
#define ITERATOREXCEPT_HH

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>
#include <boost/call_traits.hpp>

#include "ExceptionBase.hh"

using std::string;
using boost::lexical_cast;
using boost::InputIteratorConcept;
using boost::BidirectionalIteratorConcept;
using boost::call_traits;

/** \addtogroup ExceptMod Exception Module
 *  @{ */

/*! \brief Base exception class for all \c Iterator and \c ReverseIterator exceptions.
 *
 *  This is a base exception class for all of the \c Iterator and \c ReverseIterator
 *  exceptions.  It adds no functionality to the \c ExceptionBase class. */
class IteratorBaseException : public ExceptionBase
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
	IteratorBaseException(LineNumType line_number, const char* filename)
		: ExceptionBase(line_number, filename)
	{ }
};

/*! \brief Exception thrown when an out-of-range \c Iterator or 
 *         \c ReverseIterator is dereferenced.
 *
 *  This exception is thrown when an out-of-range \c Iterator or 
 *  \c ReverseIterator is dereferenced. */
class Iterator_DereferenceOutOfRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   Iterator_DereferenceOutOfRange(LineNumType line_number,
                                  const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */
   virtual string msg() const 
   {
      return "Attempted to dereference an out-of-range iterator";
   }
   
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */
   virtual string what() const 
   {
      return "DereferenceOutOfRange";
   }
};

/*! \brief Exception thrown when an \c Iterator is incremented out of range.
 *
 *  This exception is thrown when an \c Iterator is incremented beyond the range
 *  of its container. */
class Iterator_IncrementBeyondRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   Iterator_IncrementBeyondRange(LineNumType line_number,
                                 const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */   
   virtual string msg() const 
   {
      return "Attempted to increment an iterator beyond Container::end()";
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */   
   virtual string what() const 
   {
      return "IncrementBeyondRange";
   }
};

/*! \brief Exception thrown when an \c Iterator is decremented out of range.
 *
 *  This exception is thrown when an \c Iterator is decremented beyond the range
 *  of its container. */
class Iterator_DecrementBeyondRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   Iterator_DecrementBeyondRange(LineNumType line_number,
                                 const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */      
   virtual string msg() const 
   {
      return "Attempted to decrement an iterator beyond Container::begin()";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */      
   virtual string what() const 
   {
      return "DecrementBeyondRange";
   }
};

/*! \brief Exception thrown when an \c Iterator or \c ReverseIterator is 
 *         advanced out of range.
 *
 *  This exception is thrown when an \c Iterator or \c ReverseIterator is 
 *  advanced beyond the range of its container. */
class Iterator_AdvanceBeyondRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown and the distance advanced.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown
    *  and the distance of the attempted advance.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. 
    *  \param[in] n            The distance of the attempted advance. */
   Iterator_AdvanceBeyondRange(LineNumType line_number,
                      const char* filename,
                      UnsignedInt4 n)
      : IteratorBaseException(line_number, filename),
        mAdvanceDistance(n)
   {  }
   
   /*! \brief Returns the distance of the attempted advance.
    *
    *  Returns the distance of the attempted advance.
    *  \return  The distance of the attempted advance. */
   UnsignedInt4 getAdvanceDistance() const 
   { 
      return mAdvanceDistance; 
   }
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */         
   virtual string msg() const
   {
      return string("Attempted to advance a distance ")
                        + lexical_cast<string>(mAdvanceDistance)
                        + " which moved the iterator out of the Container's range";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */         
   virtual string what() const 
   {
      return "AdvanceBeyondRange";
   }
private:
   /// Stores the distance of the attempted advance.
   UnsignedInt4 mAdvanceDistance;
};
   
   
/*! \brief Exception thrown when a \c ReverseIterator is incremented out of range.
 *
 *  This exception is thrown when a \c ReverseIterator is incremented beyond the 
 *  range of its container. */   
class RevIter_ReverseIncrementBeyondRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   RevIter_ReverseIncrementBeyondRange(LineNumType line_number,
                                       const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */            
   virtual string msg() const
   {
      return "Attempted to increment a reverse iterator beyond Container::rend()";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */            
   virtual string what() const 
   {
      return "ReverseIncrementBeyondRange";
   }
};

/*! \brief Exception thrown when a \c ReverseIterator is decremented out of range.
 *
 *  This exception is thrown when a \c ReverseIterator is decremented beyond the 
 *  range of its container. */   
class RevIter_ReverseDecrementBeyondRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */
   RevIter_ReverseDecrementBeyondRange(LineNumType line_number,
                                       const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }
   
   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */               
   virtual string msg() const
   {
      return "Attempted to decrement a reverse iterator beyond Container::rbegin()";
   }
   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */               
   virtual string what() const 
   {
      return "ReverseDecrementBeyondRange";
   }
};

/*! \brief Exception thrown when an \c Iterator needs a container and doesn't have one.
 *
 *  This exception is thrown when an \c Iterator performs an operation that requires
 *  range-checking, but the \c Iterator has no container assigned to it. */
class Iterator_CheckedIteratorLacksContainer : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */  
   Iterator_CheckedIteratorLacksContainer(LineNumType line_number,
                                          const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */                 
   virtual string msg() const
   {
      return "Attempted to manipulate a checked iterator that does not point to a container";
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding a helpful error message. */                 
   virtual string what() const 
   {
      return "CheckedIteratorLacksContainer";
   }
};

/*! \brief Exception thrown when an \c Iterator is assigned a position out of the range
 *         of its container.
 *
 *  This exception is thrown when an \c Iterator is assigned a position out of the range
 *  of its container. */
class Iterator_AssignmentOutOfRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */    
   Iterator_AssignmentOutOfRange(LineNumType line_number,
                                 const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */                 
   virtual string msg() const
   {
      return "Attempted to assign an out-of-range iterator";
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */                   
   virtual string what() const 
   {
      return "AssignmentOutOfRange";
   }
};

/*! \brief Exception thrown when an \c Iterator is created outside the range of its
 *         container.
 *
 *  This exception is thrown when an \c Iterator is created outside the range of its
 *  container. */
class Iterator_ConstructionOutOfRange : public IteratorBaseException
{
public:
   /// Imports the \c LineNumType from the \c IteratorBaseException base class.
   typedef IteratorBaseException::LineNumType  LineNumType;

	/*! \brief Constructor.  Takes in the line number and filename where the exception 
	 *         was thrown.
    *
    *  Constructor.  Takes in the line number and filename where the exception was thrown.
    *  \param[in] line_number  The line number where the exception was thrown.
    *  \param[in] filename     The filename where the exception was thrown. */       
   Iterator_ConstructionOutOfRange(LineNumType line_number,
                                   const char* filename)
      : IteratorBaseException(line_number, filename)
   {  }

   /*! \brief Returns a helpful error message.
    *
    *  Returns a \c string holding a helpful error message.
    *  \return A \c string holding a helpful error message. */                    
   virtual string msg() const
   {
      return "Attempted to construct an out-of-range iterator";
   }

   /*! \brief Returns the name of the exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */                      
   virtual string what() const
   {
      return "ConstructionOutOfRange";
   }
};

  
/// @}

#endif


