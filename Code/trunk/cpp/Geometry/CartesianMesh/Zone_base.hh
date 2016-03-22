// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Zone_base.hh
 *  \author Greg Davidson
 *  \date June 3, 2005
 * 
 *  \brief Provides the \c Zone_base class, which provides the functionality common to \c Zone 
 *         classes of all spatial dimensions.
 *
 *  This file provides the \c Zone_base template class, which provides the functionality
 *  that is common to \c Zone classes of all spatial dimensions. */

#ifndef ZONE_BASE_HH
#define ZONE_BASE_HH

#include <ostream>
#include <boost/utility.hpp>

#include "Dimension.hh"
#include "MeshTraits.hh"
#include "GeomId.hh"
#include "Assert.hh"
#include "Corner.hh"

using std::ostream;

/*! \addtogroup MeshMod Mesh Subsystem
 *  @{  */

/*! \brief Provides the functionality that is common to all \c Zone types of
 *         all spatial dimensions.
 *
 *  The \c Zone_base template class provides functionality that is common to all
 *  \c Zone types of all spatial dimensions, which they receive through
 *  inheritance.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> Provides the spatial dimension spanned by the \c Zone. 
 *               Must satisfy the \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class Zone_base : public boost::noncopyable
{
private:
   /// Requires that the \c dimension_type satisfies the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);
   
public:
   /// Aliases the \c dimension_type as the \c DimensionType type.
   typedef dimension_type                                  DimensionType;
   /// Uses the \c MeshTraits<DimensionType>::LengthType as a \c LengthType.
	typedef typename MeshTraits<DimensionType>::LengthType  LengthType;
   /// Uses the \c MeshTraits<DimensionType>::SizeType as a \c SizeType.
	typedef typename MeshTraits<DimensionType>::SizeType    SizeType;
   /// Uses the \c MeshTraits<DimensionType>::IndexType as a \c IndexType.
	typedef typename MeshTraits<DimensionType>::IndexType   IndexType;
	
	/// Defines an identification number type.
	typedef GeomId< Zone<DimensionType> >                   Id;
	/// Defines the \c Node type for connectivity.
	typedef Node<DimensionType>                             Node;
	/// Defines the \c Corner type for connectivity.
	typedef Corner<DimensionType>                           Corner;
	/// Uses the \c MeshTraits<DimensionType>::PointType as a \c PointType.
	typedef typename MeshTraits<DimensionType>::PointType   PointType;
	
	/*! \brief Returns the \c Id for the \c Zone object.
	 *
	 *  Returns the \c Id for the \c Zone object.
	 *  \return Returns the \c Id for the \c Zone object. */
	const Id& id() const 
	{ 
	   return mId; 
	}
	/*! \brief Prints the \c Zone \c Id to an output stream.
	 *
	 *  Prints the \c Zone \c Id to an output stream.
	 *  \param[in] out  The output stream to print to.
	 *  \return         A reference to the output stream. */
	virtual ostream& printId(ostream& out) const
   {
	   return out << mId;
	}
	
   /*! \brief Returns the length of the \c Zone object.
    *
    *  Returns the length (x-axis size) of the \c Zone object.
    *  \return The length (x-axis size) of the \c Zone object. */
	LengthType length() const 
	{ 
	   return mLength; 
	}

protected:
   /*! \brief Constructor.  Takes an \c Id number an a length.
    *
    *  Constructor.  Takes an \c Id number and a length for the \c Zone object.
    *  This constructor is protected so that only \c Zone classes that inherit
    *  from this class may use the constructor.
    *  \param[in] id_in      The identification number for the \c Zone object.
    *  \param[in] length_in  The length for the \c Zone object. */
   Zone_base(const Id& id_in, LengthType length_in)
      : mId(id_in), mLength(length_in)
   {  }

   /// The identification number for the \c Zone object.
   Id mId;
   /// The length of the \c Zone object.
	LengthType mLength;

private:
   /// The default constructor is private to prevent its use.
   Zone_base();
};


/*! \brief This class template prevents instantiation of general-dimensional 
 *         \c Zone objects.
 *
 *  This class template prevents instantiation of general dimensional \c Zone
 *  objects by providing only private constructors. \c Zone objects must be
 *  instantiated through template specializations of this class.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> Provides the spatial dimension spanned by the 
 *               \c Zone. </dd> </dl> */
template<typename dimension_type>
class Zone : public Zone_base<dimension_type>, boost::noncopyable
{ 
private:
   /// The default constructor is private to prevent instantiation.
   Zone();
};


///  @}

#endif

