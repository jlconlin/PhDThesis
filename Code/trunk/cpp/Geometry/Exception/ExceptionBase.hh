// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file ExceptionBase.hh
 *  \author Greg Davidson
 *  \date June 3, 2005
 * 
 *  \brief Provides the abstract \c ExceptionBase base class.
 *
 *  This file provides the abstract \c ExceptionBase base class.  All other exception 
 *  classes defined in this project are derived from this class.  This class forces the 
 *  exception classes to provide a minimal amount of debugging information. */
 
#ifndef EXCEPTIONBASE_HH
#define EXCEPTIONBASE_HH

#include <exception>
#include <string>

#include "Types.hh"

using std::exception;
using std::string;

/// \defgroup ExceptMod Exception Module
/*! \addtogroup ExceptMod Exception Module
 *  \brief  Holds all of the exception classes for this project.
 *  
 *  The Exception Module consists of all of the exception classes used by this project.
 *  All of the exception classes are derived from the \c ExceptionBase class, which makes
 *  catching exceptions emanating from the Exception Module alone possible.
 *  @{ */

/*! \brief This is the abstract base class from which all exception classes are derived.
 *
 *  This class provides a base class from which all exception classes in this project are
 *  derived.  By creating a class hierarchy of exception classes we can guarantee two things.
 *  First, we can guarantee that all exception classes will store the line number and file
 *  from which they were thrown, in addition to a (hopefully) helpful message string to aid
 *  in debugging.  Second, by catching this and all child classes in catch statements, we can
 *  automatically discern between exceptions stemming from the STL or other libraries, and
 *  exceptions provided by this code. */
class ExceptionBase 
{
public:
   /// Defines a \c LineNumType for storing line numbers.
	typedef UnsignedInt4  LineNumType;
	
	/*! \brief Constructor.  Takes in a line number and filename.
	 *
	 *  Constructor taking the line number and filename from which the exception is
	 *  thrown.
	 *  \param[in] line_number  The line number where the exception was thrown.
	 *  \param[in] file         The file where the exception was thrown. */
	ExceptionBase(LineNumType line_number = 0, const char* filename = "")
		: mLineNumber(line_number), mFile(filename) 
   {  }

	/// Destructor.
	virtual ~ExceptionBase() 
	{  }

	/*! \brief Returns the line number from which the exception was thrown.
	 *
	 *  Returns the line number from which the exception was thrown.
	 *  \return  The line number from which the exception was thrown. */
	LineNumType line() const throw() 
	{ 
	   return mLineNumber; 
	}
	
	/*! \brief Returns the filename from which the exception was thrown.
	 *
	 *  Returns the filename from which the exception was thrown.
	 *  \return  The filename from which the exception was thrown. */
	const char* file() const throw() 
	{ 
	   return mFile; 
	}
	
	/*! \brief Prints out a diagnostic message.
	 *
	 *  Returns a \c string holding a diagnostic message.
	 *  \return  A \c string holding a diagnostic message. */
	virtual string msg() const = 0;
	
   /*! \brief Prints out the name of this exception.
    *
    *  Returns a \c string holding the name of the exception.
    *  \return A \c string holding the name of the exception. */
   virtual string what() const = 0;

private:
   /// Stores the line number where the exception was thrown.
	LineNumType mLineNumber;
   /// Stores the filename where the exception was thrown.
	const char* mFile;
};

/// @}

#endif	

