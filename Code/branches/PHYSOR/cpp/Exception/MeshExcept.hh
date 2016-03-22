//--------------------------------------------*-C++-*------------------------------------//
/// \file: MeshExcept.hh
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides all of the exceptions required by the Mesh subsystem.
///
/// This file provides all of the exceptions required by the Mesh subsystem, which includes
/// the Mesh class as well as the geometric sub-elements Node and Zone.  The exception classes
/// all derive from ExceptionBase.  Three exception classes are provided.  
/// The NoZoneConnected class is thrown when attempting to reference a zone that does not exist,
/// such as asking for the left Zone from a Node on the left boundary.  The NoSuchElement
/// exception is thrown by numCentering if a non-existing geometric sub-element is given to it
/// as a template parameter.  Finally, IteratorOutOfRange is thrown whenever an attempt is made
/// to dereference an iterator that does not point to a valid object.
#ifndef MESHEXCEPT_HH
#define MESHEXCEPT_HH

#include "ExceptionBase.hh"

#include <sstream>

using std::ostringstream;

/** \addtogroup ExceptSys Exception Subsystem
 *  @{
 */
/// \brief  Thrown when a non-existing Zone is referenced through connectivity.
/// 
/// The NoZoneConnected class is thrown when attempting to reference a Zone that does not exist,
/// such as asking for the left Zone from a Node on the left boundary.
class NoZoneConnected : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	NoZoneConnected(unsigned int line_number, const string& file_in)
			: ExceptionBase(line_number, file_in) { }

	/// Prints out a helpful message.
	string msg() const
	{ 
		ostringstream out;
		out << "Attempted to reference a non-existant zone at line "
			<< line() << " in file " << file();
		return out.str();
	}
};

/// \brief  Thrown when a non-existing Corner is referenced through connectivity.
///
/// The NoCornerConnected class is thrown when attempting to reference a Corner that does not exist,
/// such as asking for the left Corner from a Node on the left boundary.
class NoCornerConnected : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	NoCornerConnected(unsigned int line_number, const string& file_in)
			: ExceptionBase(line_number, file_in) { }

	/// Prints out a helpful message.
	string msg() const
	{
		ostringstream out;
		out << "Attempted to reference a non-existant corner at line "
			<< line() << " in file " << file();
		return out.str();
	}
};


/// \brief  Thrown when numCentering is given a non-existing geometric sub element.
/// 
/// The NoSuchElement exception is thrown by numCentering if a non-existing geometric sub-element 
/// is given to it as a template parameter.
class NoSuchElement : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception was thrown.
	NoSuchElement(unsigned int line_number, const string& file_in)
		: ExceptionBase(line_number, file_in) { }
	
	/// Prints out a helpful message.
	string msg() const
	{ 
		ostringstream out;
		out << "Attempted to reference a non-existant element at line "
			<< line() << " in file " << file();
		return out.str();
	}
};

///  @}

#endif
