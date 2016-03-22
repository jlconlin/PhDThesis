// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Angle1D.hh
 *  \author Greg Davidson
 *  \date June 14, 2008
 * 
 *  \brief Provides the one-dimensional \c Angle class, which holds angular ordinates and weights.
 *
 *  This file provides the one-dimensional \c Angle class. This class stores an angular ordinate
 *  and weight. */
#ifndef ANGLE1D_HH
#define ANGLE1D_HH

#include "Angle_base.hh"
#include "GeomVector1D.hh"


/*! \brief Provides a one-dimensional angular ordinate set.
 *
 *  This \c OneD template specialization of the \c Angle class provides an (angle, weight) pair 
 *  for the angular quadrature set. */
template<>
class Angle<OneD> : public Angle_base<OneD>
{
private:
   /// Defines \c Angle_base<OneD> as the \c Base type.
   typedef Angle_base<OneD>      Base;
   
public:
   /// Imports the \c AngleType from the \c Base type.
	typedef Base::AngleType       AngleType;
	/// Imports the \c WeightType from the \c Base type.
	typedef Base::WeightType      WeightType;

	/*! \brief Constructor.  Takes in an angle and a weight for this quadrature point.
	 *
	 *  Constructor.  Takes in an angle and a weight for this quadrature point.
	 *  \param[in] angle_in  The angle for this quadrature point.
	 *  \param[in] weight_in The weight for this quadrature point. */
	explicit Angle(const AngleType& angle_in = AngleType(), WeightType weight_in = WeightType() )
			: Angle_base<OneD>(angle_in, weight_in)
	{  }

	/*! \brief Copy Constructor.
	 *
	 *  Copy constructor.
	 *  \param[in] angle_in  Makes a copy of angle_in. */
	Angle(const Angle<OneD>& angle_in)
			: Angle_base<OneD>(angle_in.mAngle, angle_in.mWeight)
	{  }
	
   /*! \brief Copy operator.
    *
    *  Copy operator.
    *  \param[in] angle_in  The \c Angle<OneD> we are copying.
    *  \return  This object after copying. */
   Angle<OneD>& operator=(const Angle<OneD>& angle_in)
   {
      Base::mAngle.i()  = angle_in.mAngle.i();
      Base::mWeight = angle_in.mWeight;
      
      return *this;
   }
	
};

/// Define the \c Angle1D type as an \c Angle<OneD>.
typedef Angle<OneD>  Angle1D;

#endif


