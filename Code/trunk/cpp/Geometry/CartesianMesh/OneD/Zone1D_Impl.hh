// Date Last Altered: $Date: 2009-03-10 10:56:40 -0600 (Tue, 10 Mar 2009) $
// Revision Number: $Revision: 480 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Zone1D_Impl.hh
 *  \author Greg Davidson
 *  \date June 23, 2008
 * 
 *  \brief Provides the implementation of certain methods for the Zone<OneD> class.
 *
 *  This file provides the implementation of certain methods for the Zone<OneD> class. */

#ifndef ZONE1D_IMPL_HH
#define ZONE1D_IMPL_HH

#include <cmath>
#include <limits>

#include "Angle1D.hh"
#include "Node1D.hh"
#include "Corner.hh"

using std::fabs;
using std::numeric_limits;

/*! Returns the \c PointType located at the \c Zone object's center.
 *  \return The \c PointType located at the \c Zone object's center. */
inline Zone<OneD>::PointType Zone<OneD>::center() const
{
   Assert(mNodes.first != 0);
   Assert(mNodes.second != 0);
   
   return PointType( (mNodes.first->x() + mNodes.second->x()) / 2.0 );
}

/*! Calculates the distance to the boundary of the Zone from \a position in direction \a angle.
 *  \param[in]  position  The position to measure the distance from.
 *  \param[in]  angle     The direction to measure the distance.
 *  \return  The distance from \a position along direction \a angle. */
inline Zone<OneD>::LengthType Zone<OneD>::distanceToBndry(const Point<OneD>& position, 
                                                          const Angle<OneD>& angle) const
{
	Assert( fabs(angle.mu()) 
	               >= numeric_limits< Angle<OneD>::AngleType::OrdinateType >::epsilon() );
	Assert(mNodes.first == 0);
	Assert(mNodes.second == 0);
	               
   Zone<OneD>::LengthType node_pos;
	if(angle.mu() < 0.0)
      node_pos = mNodes.first->x();
   else
   	node_pos = mNodes.second->x();
   return (node_pos - position.x()) / angle.mu();   
}

#endif

