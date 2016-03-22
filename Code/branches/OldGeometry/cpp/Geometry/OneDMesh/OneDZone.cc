// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: OneDZone.cc
/// \author Greg Davidson
/// \date June 3, 2005
/// 
/// \brief This file provides the implementation for the one-dimensional Zone geometric sub-element.
///
/// This file provides the implementation for the one-dimensional Zone class.  The Zone class
/// basically stores all of the information that you would expect a zone to contain (basically
/// just physical dimensions).  The Zone has references to its left and right Nodes as well.

#include <limits>
#include <cmath>
#include <iostream>

#include "OneDZone.hh"
#include "Assert.hh"

using std::cout;
using std::endl;
using std::fabs;
using std::numeric_limits;

/// Gets one Node connectivity.
///
/// This method returns one node connectivity for the zone.  The parameter \a index requires
/// some additional explanation.
/// \param index
/// When \a index =0, the method will return the left node.  When \a index = 1, the method will
/// return the right node.
const Node& Zone::getNode(unsigned int index) const
{
	// Ensure that index is either 0 or 1.
	Assert(index <= 1);
	
	if(index == 0) // Left node
	{
		return *(mNodes.first());
	}
	else // Right node
	{
		return *(mNodes.second());
	}
}

/// Gets one Corner connectivity.
///
/// This method returns one Corner connectivity for the Zone.  The parameter \a index requires
/// some additional explanation.
/// \param index
/// When \a index =0, the method will return the left Corner.  When \a index = 1, the method will
/// return the right Corner.
const Corner& Zone::getCorner(unsigned int index) const
{
	// Ensure that index is either 0 or 1.
	Assert(index <= 1);

	if(index == 0)
	{
		return *(mCorners.first());
	}
	else // Right node
	{
		return *(mCorners.second());
	}
}


/// Calculates the distance to the boundary of the zone.
///
/// Given a position \a x_pos (which is assumed to be within the zone) this method calculates
/// the distance along an angle \a angle from \a x_pos to the boundary of the zone.
/// \warning
/// This function will not work correctly if \a x_pos is not actually located within the zone.
double Zone::distanceToBndry(double x_pos, double angle) const
{

	if(fabs(angle) <= numeric_limits<double>::epsilon() )
	{
		cout << angle << endl;
		Assert(fabs(angle) >= numeric_limits<double>::epsilon() );
	}
	
	double node_pos = 0.0;
	if(angle < 0.0)
	{
		node_pos = mNodes.first()->x(); // Left node
	}
	else
	{
		node_pos = mNodes.second()->x(); // Right node
	}
	
	return (node_pos - x_pos)/angle;
}

/// Calculates the position of the center of the zone
double Zone::center() const
{
	return (mNodes.first()->x() + mNodes.second()->x())/2.0;
}
