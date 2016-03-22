// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file: Point1D.hh
 *  \author Greg Davidson
 *	 \date August 4, 2007
 *
 *  \brief Provides the \c Point<OneD> class for storing one-dimensional points.
 *
 *  This file provides the \c Point<OneD> class.  The \c Point<OneD> class stores 
 *  points in a one-dimensional space. */

#ifndef POINT1D_HH
#define POINT1D_HH

#include "Point_base.hh"

/*! \addtogroup MeshMod Mesh Module
 *  @{  */


/*! \brief Provides a class for storing points in one-dimensional space.
 *
 *  The The \c Point<OneD> class stores points in a one-dimensional space. It inherits
 *  some of its functionality from the \c Point_base<OneD> class.  */
template<>
class Point<OneD> : public Point_base<OneD>
{
private:
   /// Aliases the \c Point_base<OneD> as the \c Base type.
   typedef Point_base<OneD>   Base;
public:
   /// Imports the \c Point_base<OneD>::PointType into the \c Point<OneD> scope.
	typedef Base::PointType    PointType;

   /*! \brief Constructor.  Takes in the x-position for the \c Point<OneD> object.
    *
    *  Constructor.  Takes in the x-position for the \c Point<OneD> object.
    *  \param[in] x_in  The x-position for the \c Point<OneD> object. */ 
	explicit Point(PointType x_in = 0.0)
	   : Point_base<OneD>(x_in)
	{  }

   /*! \brief Copy constructor.
    *
    *  Copy constructor.
    *  \param[in]  point_in  The \c Point<OneD> to copy from. */
	Point(const Point& point_in) 
	   : Point_base<OneD>(point_in.mX) 
	{  }
	
	/*! \brief Copy operator.
	 *
	 *  Copy operator.
	 *  \param[in] point_in  The \c Point<OneD> to copy from.
	 *  \return    A reference to this \c Point<OneD> object. */
	Point& operator=(const Point& point_in)
	{
      Base::mX = point_in.mX;
      return *this;
   }
};

/// @}

#endif

