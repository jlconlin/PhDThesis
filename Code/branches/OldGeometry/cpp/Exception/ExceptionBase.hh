//--------------------------------------------*-C++-*------------------------------------//
/// \file: ExceptionBase.hh
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides the abstract exception base class.
///
/// This file provides the abstract exception base class.  All other exception classes
/// defined in this project are derived from this class.  This class forces the exception
/// classes to provide a minimal amount of debugging information.
#ifndef EXCEPTIONBASE_HH
#define EXCEPTIONBASE_HH

#include <string>

using std::string;

/*! \addtogroup ExceptSys Exception Subsystem
 *  The Exception Sybsystem consists of all of the exception classes used by this project.
 *  All of the exception classes are derived from the ExceptionBase class, which makes
 *  catching exceptions emanating from the Exception Subsystem alone possible.
 *  @{
 */

/// \brief This is the abstract base class from which all exception classes are derived.
///
/// This class provides a base class from which all exception classes in this project are
/// derived.  By creating a class hierarchy of exception classes we can guarantee two things.
/// First, we can guarantee that all exception classes will store the line number and file
/// from which they were thrown, in addition to a (hopefully) helpful message string to aid
/// in debugging.  Second, by catching this and all child classes in catch statements, we can
/// automatically discern between exceptions stemming from the STL or other libraries, and
/// exceptions provided by this code.
class ExceptionBase
{
public:
	/// The constructor takes the line number and filename from which the exception was thrown.
	ExceptionBase(unsigned int line_number = 0, const string& file_in = string())
		: mLineNumber(line_number), mFile(file_in) { }

	/// Destructor.
	virtual ~ExceptionBase() { }

	/// Returns the line number from which the exception was thrown.
	unsigned int line() const { return mLineNumber; }
	/// Returns the filename from which the exception was thrown.
	const string& file() const { return mFile; }
	
	/// Prints out a diagnostic message.
	virtual string msg() const = 0;

private:
	unsigned int mLineNumber; ///< Stores the line number where the exception was thrown.
	const string mFile; ///< Stores the filename where the exception was thrown.
};

/// @}

#endif	
