//--------------------------------------------*-C++-*------------------------------------//
/// \file: MethodExcept.hh
/// \author $Author: wollaber $
/// \date   $Date: 2007-07-25 22:01:34 -0400 (Wed, 25 Jul 2007) $
/// Id      $Id: MethodExcept.hh 117 2007-07-26 02:01:34Z wollaber $
/// 
/// \brief This file provides the Method exception classes.
///
/// This file provides the Method exception classes.  These classes get thrown
/// whenever Method (or one of its derived classes) receives a bad instruction 
/// that may generally be corrected through
/// additional user intervention.

#ifndef METHOD_EXCEPT_HH
#define METHOD_EXCEPT_HH

#include "ExceptionBase.hh"

#include <sstream>

using std::ostringstream;

/** \addtogroup ExceptSys Exception Subsystem
 *  @{
 */
/// \brief InvalidData is thrown if invalid data is passed to Method or one of its derived classes.
///
/// InvalidData is thrown if invalid data is passed to Method or one of its derived classes.  For instance,
/// Diffusion must have nonzero opacities to generate diffusion coefficients.  
class InvalidData : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	InvalidData(unsigned int line_number, const string& file)
		: ExceptionBase(line_number, file) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Invalid data passed to a transport Method.  Exception thrown from line "
			<< line() << " in file " << file();
		return out.str();
	}
};


/// \brief AdvancedTooFar is thrown if a Method is asked to calculate past the last time step.
///
/// AdvancedTooFar is thrown if a Method is asked to calculate past the last time step.
class AdvancedTooFar : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	AdvancedTooFar(unsigned int line_number, const string& file)
		: ExceptionBase(line_number, file) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Method has reached the last time step for this problem. Exception thrown from line "
			<< line() << " in file " << file();
		return out.str();
	}
};

/// \brief ModelingError is thrown if a Method commits a modeling error.
///
/// ModelingError is thrown if a Method is commits a modeling error.  An example might be
/// if energy is not conserved or if a probability distribution isn't normalized. 
class ModelingError : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	ModelingError(unsigned int line_number, const string& file)
		: ExceptionBase(line_number, file) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Method has committed a modeling error. Exception thrown from line "
			<< line() << " in file " << file();
		return out.str();
	}
};



/// @}

#endif


