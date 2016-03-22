// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file: Point_base.hh
 *  \author Greg Davidson
 *	 \date August 4, 2007
 *
 * \brief Provides the Point_base class that provides functionality for \c Point objects
 *        of all dimensions.
 *
 * This file provides the \c Point_base class.  The \c Point_base class implements the
 * functionality that is common to all \c Point objects of any spatial dimension. */

#ifndef POINT_BASE_HH
#define POINT_BASE_HH

#include <boost/utility.hpp>

#include "Dimension.hh"

/*! \addtogroup MeshMod Mesh Subsystem
 *  @{  */

/*! \brief Provides the functionality common to \c Point objects of all spatial
 *  dimensionality.
 *
 *  This class implements the functionality common to \c Point objects of all
 *  spatial dimensionality, which they recieve through inheritance.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c Point_base spans.  Must fulfill
 *               the \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class Point_base : public boost::noncopyable
{
private:
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);
   
public:
   /// Aliases the \c dimension_type as a \c DimensionType.
   typedef dimension_type     DimensionType;
   /// Defines the \c PointType as a real number.
   typedef Real8              PointType;

	/*! \brief Returns a reference to the x-position of the 
	 *         \c Point object.
	 *
	 *  Returns a reference to the x-position of the \c Point object.
	 *  \return A reference to the x-position of the \c Point object. */
	PointType& x()
	{
	   return mX;
	}
	/*! \brief Returns a copy of the x-position of the \c Point object.
	 *
	 *  Returns a copy of the x-position of the \c Point object.
	 *  \return A copy of the x-position of the \c Point object. */
	PointType x() const
	{
	   return mX;
	}

protected:
   /*! \brief Constructor.  Takes in the x-position for the \c Point object.
    *
    *  Constructor.  Takes in the x-position for the \c Point object.  This
    *  constructor is protected so that only \c Node objects that inherit
    *  from this class may use the constructor.
    *  \param[in] x_in  The x-position for the \c Point object. */
   Point_base(PointType x_in)
      : mX(x_in)
   {  }
   
	/// Stores the x-position of the \c Point object.
	PointType mX;

private:
   /// The default constructor is private to prevent instantiation.
   Point_base();
};
 
/*! \brief General template class for the \c Point type.  No public
 *         constructors.
 *
 *  This is the general template class for the \c Point type.  This
 *  class has no public constructors so that it can not be
 *  instantiated.  Only template specializations of this class may
 *  be instantiating.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c Point spans.  Must fulfill
 *               the \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class Point : public Point_base<dimension_type>, boost::noncopyable
{
private:
   /// The default constructor is private to prevent instantiation.
   Point();   
};

/// @}

#endif

