// Date Last Altered: $Date$
// Revision Number: $Revision$
//--------------------------------------------*-C++-*------------------------------------//
/// \file: OneDNode.cc
/// \author Greg Davidson
/// \date June 8, 2005
/// 
/// \brief This file provides the implementation for the one-dimensional Node geometric sub-element.
///
/// This file provides the implementation for the one-dimensional Node class.  The Node class
/// stores positional information and references to its connected Zones.

#include "OneDNode.hh"
#include "Assert.hh"

/// Gets one Zone connectivity.
///
/// This method returns one Zone connectivity for the Node.  If the pointer is null, the method
/// throws the NoZoneConnected exception.  The parameter \a index requires
/// some additional explanation.
/// \param index
/// When \a index =0, the method will return the left Zone.  When \a index = 1, the method will
/// return the right Zone. 
const Zone& Node::getZone(unsigned int index) const throw(NoZoneConnected)
{
	// Ensure that index is either 0 or 1.
	Assert(index <= 1);
	
	if(index == 0) // Left Zone
	{
		if(mZones.first() == 0) // There is no left Zone
		{
			throw(NoZoneConnected(__LINE__, __FILE__) );
		}
		return *mZones.first();
	}
	else // Right Zone
	{
		if(mZones.second() == 0) // There is no right Zone
		{
			throw(NoZoneConnected(__LINE__, __FILE__) );
		}
		return *mZones.second();
	}
}


/// Gets one Corner connectivity.
///
/// This method returns one Corner connectivity for the Node.  If the pointer is null, the method
/// throws the NoCornerConnected exception.  The parameter \a index requires
/// some additional explanation.
/// \param index
/// When \a index =0, the method will return the left Corner.  When \a index = 1, the method will
/// return the right Corner. 
const Corner& Node::getCorner(unsigned int index) const throw(NoCornerConnected)
{
	// Ensure that index is either 0 or 1.
	Assert(index <= 1);

	if(index == 0) // Left Corner
	{
		if(mCorners.first() == 0) // There is no left Corner
		{
			throw(NoCornerConnected(__LINE__, __FILE__) );
		}
		return *mCorners.first();
	}
	else // Right Corner
	{
		if(mCorners.second() == 0) // There is no right Corner
		{
			throw(NoCornerConnected(__LINE__, __FILE__) );
		}
		return *mCorners.second();
	}
}
