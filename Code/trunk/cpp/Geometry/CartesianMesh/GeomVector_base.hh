// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file GeomVector_base.hh
 *  \author Greg Davidson
 *	 \date August 4, 2007
 *
 *  \brief Provides the \c GeomVector_base base class for the \c GeomVector class.
 *
 *  This file provides the \c GeomVector_base class.  The \c GeomVector_base class implements
 *  certain functionality that is universal to \c GeomVector types of all spatial dimensions. */

#ifndef GEOMVECTOR_BASE_HH
#define GEOMVECTOR_BASE_HH

#include <boost/utility.hpp>
#include <boost/concept_check.hpp>

#include "Dimension.hh"
#include "Types.hh"

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/*! \brief Provides the \c GeomVector_base base class for the \c GeomVector class.
 *
 *  The \c GeomVector_base class is a base class for the \c GeomVector class.  The
 *  \c GeomVector_base class implements all of the functionality that \c GeomVector classes
 *  of all spatial dimensions need.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The type that gives the spatial dimension of the \c GeomVector_base type.  This
 *               type must satisfy the \c DimensionConcept. </dd> </dl> */
template<typename dimension_type>
class GeomVector_base : public boost::noncopyable
{
private:
   /// This forces \c dimension_type to satisfy the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);

public:
	/// Aliases the \c dimension_type as \c DimensionType.
	typedef dimension_type  DimensionType;
	/// Defines the \c OrdinateType as a real number.
	typedef Real8           OrdinateType;

	/*! \brief Returns a reference to the x-axis ordinate data.
	 *
	 *  Returns a reference to the x-axis ordinate data.
	 *  \return A reference to the x-axis ordinate data. */
	OrdinateType& i() 
	{ 
	   return mI; 
	}
	/*! \brief Returns a copy of the x-axis ordinate data.
	 *
	 *  Returns a copy of the x-axis ordinate data.
	 *  \return A copy of the x-axis ordinate data. */
	OrdinateType i() const 
	{ 
	   return mI; 
	}

protected:
   /*! \brief Constructor taking the x-axis ordinate data.
    *
    *  Constructor.  Takes in the x-axis ordinate data.  This constructor
    *  is protected so that \c GeomVector_base may not be
    *  instantiated.
    *  \param[in] i_in  The x-axis ordinate for the GeomVector_base object. */
   explicit GeomVector_base(OrdinateType i_in)
      : mI(i_in)
   {  }
   
	/// Stores the \c GeomVector_base x-axis ordinate data.
	OrdinateType mI;

private:
	/// The default constructor is private to prevent instantiation.
	GeomVector_base();
};

/*! \brief General template declaration for the \c GeomVector class.
 *
 *  This is the general template declaration fo the \c GeomVector class. This
 *  class has no public constructors so that it can not be instantiated.  Only
 *  template specializations of this class may be instantiated.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The type giving the spatial dimension that the \c GeomVector
 *               spans. </dd> </dl> */
template<typename dimension_type>
class GeomVector : public GeomVector_base<dimension_type>, boost::noncopyable
{
private:
   /// This constructor is private to prevent instantiation.
   GeomVector();
};

///  @}

#endif

