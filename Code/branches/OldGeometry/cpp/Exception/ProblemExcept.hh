//--------------------------------------------*-C++-*------------------------------------//
/// \file: ProblemExcept.hh
/// \author $Author: wollaber $
/// \date   $Date: 2007-07-25 22:01:34 -0400 (Wed, 25 Jul 2007) $
/// Id      $Id: ProblemExcept.hh 117 2007-07-26 02:01:34Z wollaber $
/// 
/// \brief This file provides the Problem exception classes.
///
/// This file provides the Problem exception classes.  These classes get thrown
/// whenever Problem receives a bad instruction that may generally be corrected through
/// additional user intervention.

#ifndef PROBLEM_EXCEPT_HH
#define PROBLEM_EXCEPT_HH

#include "ExceptionBase.hh"

#include <sstream>

using std::ostringstream;

/** \addtogroup ExceptSys Exception Subsystem
 *  @{
 */
/// \brief Thrown if an invalid initial condition is attempted to be prescribed.
///
/// InvalidInitialCondition is thrown if the user attempts to specify a negative initial condition.
/// This could later be expanded upon to become more intelligent, if desired.
class InvalidInitialCondition : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	InvalidInitialCondition(unsigned int line_number, const string& file)
		: ExceptionBase(line_number, file) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Unphysical initial condition encountered, please check.  Exception thrown from line "
			<< line() << " in file " << file();
		return out.str();
	}
};


/// \brief DimensionMismatch is thrown if the users inputs data that is inconsistent in dimension.
///
/// DimensionMismatch is thrown if the users inputs data that is inconsistent in dimension.  An example
/// of this might be if the number of problem regions does not match the number of materials, or if the
/// the number of time delta-ts does not match the number of times requested for output.  
class DimensionMismatch : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	DimensionMismatch(unsigned int line_number, const string& file)
		: ExceptionBase(line_number, file) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Dimension mismatch in input, check for consistency.  Exception thrown from line "
			<< line() << " in file " << file();
		return out.str();
	}
};

/// \brief InsaneInput is thrown if the users inputs unphysical data.
///
/// InsaneInput is thrown if the users inputs unphysical data, such as negative temperatures,
/// non-increasing time steps, etc.  
class InsaneInput : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	InsaneInput(unsigned int line_number, const string& file)
		: ExceptionBase(line_number, file) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Dimension mismatch in input, check for consistency.  Exception thrown from line "
			<< line() << " in file " << file();
		return out.str();
	}
};


/// @}

#endif


