//--------------------------------------------*-C++-*------------------------------------//
/// \file: IteratorExcept.hh
/// \author Greg Davidson
/// \date February 6, 2005
/// 
/// \brief This file provides the IteratorOutOfRange exception class.
///
/// This file provides the IteratorOutOfRange exception class.  This class gets thrown
/// anytime a checked iterator moves out of a valid container range, or attempts to
/// access a container outside a valid range.  Range checking can be turned off by
/// defining the macro \a NITERCHK
#ifndef ITERATOREXCEPT_HH
#define ITERATOREXCEPT_HH

#include "ExceptionBase.hh"

#include <sstream>

using std::ostringstream;

/** \addtogroup ExceptSys Exception Subsystem
 *  @{
 */
/// \brief Thrown when an iterator is made invalid or an invalid location is dereferenced.
///
/// IteratorOutOfRange is thrown whenever an attempt is made to dereference an iterator that 
/// does not point to a valid object, or when an iterator is moved outside the an objects
/// range.
class IteratorOutOfRange : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	IteratorOutOfRange(unsigned int line_number, const string& file_in)
		: ExceptionBase(line_number, file_in) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Attempted to dereference an invalid iterator at line "
			<< line() << " in file " << file();
		return out.str();
	}
};

/// @}

#endif


