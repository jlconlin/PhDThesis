// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Angle_base.hh
 *  \author Greg Davidson
 *  \date February 8, 2005
 * 
 *  \brief Provides the \c Angle_base class, which provides functionality
 *         universal to \c Angle classes of any spatial dimension.
 *
 *  This file provides the \c Angle_base class. This class provides the functionality 
 *  that is universal to \c Angle classes of any spatial dimension. */
 
#ifndef ANGLE_BASE_HH
#define ANGLE_BASE_HH

#include <boost/utility.hpp>

#include "Dimension.hh"
#include "GeomVector_base.hh"

/*! \addtogroup AngleMod Angular Quadrature Module
 *  @{  */
 
/*! \brief Provides the functionality needed by \c Angle classes of any spatial
 *         dimension.
 *
 *  This class provides all of the functionality that is common to all \c Angle classes
 *  regardless of what spatial dimension we are working in. 
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> A type storing the spatial dimension that we are working in.  This type
 *               must satisfy the \c DimensionConcept concept class. */
template<typename dimension_type>
class Angle_base : public boost::noncopyable
{
private:
   /// Requires that the \c dimension_type satisfy the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);

public:
   // ****** Public type declarations ******
   /// Alias the \c dimension_type template parameter as \c DimensionType.
   typedef dimension_type                    DimensionType;
   /// Sets \c GeomVector<DimensionType> as the \c AngleType.
	typedef GeomVector<DimensionType>         AngleType;
	/// Defines the \c WeightType.
	typedef Real8                             WeightType;
   /// Defines the \c OrdinateType from the \c AngleType.
   typedef typename AngleType::OrdinateType  OrdinateType;
	
	// ****** Data accessors ******
	/*! \brief  Returns a constant reference to the angle ordinate.
	 *
	 *  Returns a constant reference to the angle ordinate.
	 *  \return A constant reference to the angle ordinate. */
	const AngleType& angle() const 
	{ 
	   return mAngle; 
	}
	
	/*! \brief  Returns a reference to the angle ordinate.
	 *
	 *  Returns a reference to the angle ordinate.
	 *  \return A reference to the angle ordinate. */
	AngleType& angle() 
	{ 
	   return mAngle; 
	}
	
	/*! \brief  Returns a copy of the angle weight.
	 *
	 *  Returns a copy of the angle weight.
	 *  \return A copy of the angle weight. */
	WeightType weight() const 
	{ 
	   return mWeight; 
	}
	/*! \brief  Returns a reference to the angle weight.
	 *
	 *  Returns a reference to the angle weight.
	 *  \return A reference to the angle weight. */
	WeightType& weight() 
	{
	   return mWeight; 
	}

	/*! \brief  Returns a copy of the angle ordinate's x-axis component.
	 *
	 *  Returns a copy of the angle ordinate's x-axis component.
	 *  \return A copy of the angle ordinate's x-axis component. */
   OrdinateType mu() const
   {
      return mAngle.i();
   }
	/*! \brief  Returns a reference to the angle ordinate's x-axis component.
	 *
	 *  Returns a reference to the angle ordinate's x-axis component.
	 *  \return A reference to the angle ordinate's x-axis component. */
   OrdinateType& mu()
   {
      return mAngle.i();
   }
	
protected:
   // ****** Constructor ******
	/*! \brief Constructor taking in an \c AngleType and a weight.  
	 *
	 *  Constructor taking in an \c AngleType and a weight. Protected to prevent direct 
	 *  instantiation.
	 *  \param[in] angle_in  The \c GeomVector<Dim> to instantiate with.
	 *  \param[in] weight_in The weight for this angular ordinate. */
	Angle_base(const AngleType& angle_in, WeightType weight_in)
			: mAngle(angle_in), mWeight(weight_in)
	{  }

   /// Stores the angle for this angular ordinate.
	AngleType mAngle;  
   /// Stores the weight for this angular ordinate.                             
	WeightType mWeight;

private:
   /// Private to prevent instantiation
   Angle_base();
};


/*! \brief General dimensional \c Angle class. Prevents the instantiation of 
 *         general-dimensional \c Angle objects.
 *
 *  This class prevents the instantiation of general-dimensional Angle objects by making
 *  the constructor private. */
template<typename dimension_type>
class Angle : public Angle_base<dimension_type>, boost::noncopyable
{  
private:
   /// The default constructor is private to prevent instantiation.
   Angle();
};



/// @}

#endif

